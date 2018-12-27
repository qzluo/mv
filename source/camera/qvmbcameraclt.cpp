#include "qvmbcameraclt.h"
#include <QDebug>
#include <QThread>

#include "VimbaImageTransform/Include/VmbTransform.h"
#include "FileLogger.h"

QVmbCameraclt::QVmbCameraclt(QObject *parent) : CameraCtl(parent),
    sys( AVT::VmbAPI::VimbaSystem::GetInstance() )
{
    camWidth = 1280;
    camHeight = 960;

    bStarted = false;
    curImg = QImage();

    SP_RESET(pCamera);
}

QVmbCameraclt::~QVmbCameraclt()
{
    if (bStarted)
        StopView();
}

bool QVmbCameraclt::Initiallize(int DeviceNumber, QString paramPath)
{
    VmbErrorType err = sys.Startup();               // Initialize the Vimba API

    if ( VmbErrorSuccess != err ) {
        logFile(FileLogger::warn, "Failed to start camera api system.");
        return false;
    }

    AVT::VmbAPI::CameraPtrVector cameras;
    err = sys.GetCameras( cameras );            // Fetch all cameras known to Vimba

    if (err != VmbErrorSuccess) {
        logFile(FileLogger::warn, "Failed to get cameras.");
        sys.Shutdown();
        return false;
    }

    int cameraCount = cameras.size();

    char msg[1024] = {};
    sprintf(msg, "camera count = %d.", cameraCount);
    logFile(FileLogger::info, msg);

    if (DeviceNumber >= cameraCount) {
        sprintf(msg, "The input camera id (%d) should less than %d",
                DeviceNumber, cameraCount);
        logFile(FileLogger::warn, msg);

        sys.Shutdown();
        return false;
    }

    return true;
}

bool QVmbCameraclt::StartView()
{
    bStarted = false;

    //打开相机
    AVT::VmbAPI::CameraPtrVector cameras;
    VmbErrorType err = sys.GetCameras( cameras );
    if (VmbErrorSuccess != err || cameras.empty()) {
        sys.Shutdown();
        return false;
    }

    err = cameras[0]->Open( VmbAccessModeFull );                // Open the camera
    if( VmbErrorSuccess != err ) {
        sys.Shutdown();
        return false;
    }

    pCamera = cameras[0];

    // Set the GeV packet size to the highest possible value
    AVT::VmbAPI::FeaturePtr pCommandFeature;
    if( VmbErrorSuccess == SP_ACCESS( pCamera )->
            GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ) ) {
        if( VmbErrorSuccess == SP_ACCESS( pCommandFeature )->RunCommand() ) {
            bool bIsCommandDone = false;
            do {
                if( VmbErrorSuccess != SP_ACCESS( pCommandFeature )->
                        IsCommandDone( bIsCommandDone ) )
                {
                    break;
                }
            } while( false == bIsCommandDone );
        }
    }

    //获得图形尺寸
    AVT::VmbAPI::FeaturePtr pFeature;
    err = SP_ACCESS( pCamera )->GetFeatureByName( "Width", pFeature );
    if( VmbErrorSuccess != err ) {
        SP_ACCESS( pCamera )->Close();
        sys.Shutdown();
        return false;
    }
    SP_ACCESS( pFeature )->GetValue( camWidth );

    err = SP_ACCESS( pCamera )->GetFeatureByName( "Height", pFeature );
    if( VmbErrorSuccess != err ) {
        SP_ACCESS( pCamera )->Close();
        sys.Shutdown();
        return false;
    }
    SP_ACCESS( pFeature )->GetValue( camHeight );

    // Store currently selected image format
    err = SP_ACCESS( pCamera )->GetFeatureByName( "PixelFormat", pFeature );
    if( VmbErrorSuccess != err ) {
        SP_ACCESS( pCamera )->Close();
        sys.Shutdown();
        return false;
    }

    err = SP_ACCESS( pFeature )->GetValue( m_nPixelFormat );
    if( VmbErrorSuccess != err ) {
        SP_ACCESS( pCamera )->Close();
        sys.Shutdown();
        return false;
    }

    //init image
    curImg = QImage(camWidth, camHeight, QImage::Format_RGB888);

    // Create a frame observer for this camera
    // (This will be wrapped in a shared_ptr so we don't delete it)
    SP_SET( m_pFrameObserver , new AVT::VmbAPI::FrameObserver( pCamera ) );
    connect( SP_DYN_CAST( m_pFrameObserver, AVT::VmbAPI::FrameObserver ).get(),
             SIGNAL( FrameReceivedSignal(int) ), this, SLOT( OnFrameReady(int) ) );

    // Start streaming
    err = SP_ACCESS( pCamera )->StartContinuousImageAcquisition( NUM_FRAMES,
                                                                 m_pFrameObserver );
    if ( VmbErrorSuccess != err ) {
        SP_ACCESS( pCamera )->Close();
        sys.Shutdown();
        return false;
    }

    char msg[1024] = {};
    sprintf(msg, "Start view success.(camera width = %d, camera height = %d)",
            camWidth, camHeight);
    logFile(FileLogger::info, msg);

    bStarted = true;

    return true;
}

bool QVmbCameraclt::StopView()
{
    // Stop streaming
    SP_ACCESS( pCamera )->StopContinuousImageAcquisition();

    // Close camera
    if (!SP_ISNULL( pCamera )) {
        pCamera->Close();
        SP_RESET(pCamera);
    }

    // Clears all remaining frames that have not been picked up
    SP_DYN_CAST( m_pFrameObserver, AVT::VmbAPI::FrameObserver )->
            ClearFrameQueue();

    //关闭系统
    sys.Shutdown();

    curImg = QImage();

    bStarted = false;

    return true;
}

Mat QVmbCameraclt::GrabOneFrame()
{
    mutex.lock();
    Mat mat = ImageTransform::QImageToMat(curImg,true);
    mutex.unlock();

    return mat;
}

int QVmbCameraclt::GetCamWidth()
{
    return camWidth;
}

int QVmbCameraclt::GetCamHeight()
{
    return camHeight;
}

int QVmbCameraclt::SetPara(QString featureName, QString value)
{
    //get feature from featureName
    char msg[1024] = {};
    AVT::VmbAPI::FeaturePtr pFeature;
    VmbErrorType err = VmbErrorSuccess;
    err = SP_ACCESS( pCamera )-> GetFeatureByName( featureName.toLatin1().data(), pFeature );
    if( VmbErrorSuccess != err ) {
        sprintf(msg, "GetFeatureByName failed. errcode = %d.", err);
        logFile(FileLogger::warn, msg);
        return -1;
    }

    //check flags
    VmbFeatureFlagsType flags = VmbFeatureFlagsNone;
    if ((VmbErrorSuccess != pFeature->GetFlags(flags)) ||
            ((VmbFeatureFlagsWrite & flags) == 0)) {
        logFile(FileLogger::warn, "Featrue is not writable!");
        return -1;
    }

    //get feature type
    VmbFeatureDataType dataType = VmbFeatureDataUnknown;
    if (VmbErrorSuccess != pFeature->GetDataType(dataType)) {
        logFile(FileLogger::warn, "GetDataType failed!");
        return -1;
    }

    switch(dataType) {
    case VmbFeatureDataInt:
        break;

    case VmbFeatureDataFloat:
    {
        double dMin = 0.0;
        double dMax = 0.0;
        VmbBool_t incrementSupported = false;
        double dInc = 0.0;
        if( VmbErrorSuccess != pFeature->GetRange(dMin, dMax)) {
            sprintf(msg, "Feature %s GetRange failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        if( VmbErrorSuccess != pFeature->HasIncrement(incrementSupported)) {
            sprintf(msg, "Feature %s HasIncrement failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        if (incrementSupported) {
            if( VmbErrorSuccess != pFeature->GetIncrement(dInc)) {
                sprintf(msg, "Feature %s GetIncrement failed.",
                        featureName.toLatin1().data());
                logFile(FileLogger::warn, msg);
                return -1;
            }
        }

        double dValue = value.toDouble();
        if (dValue < dMin)
            dValue = dMin;
        else if (dValue > dMax)
            dValue = dMax;
        else if (incrementSupported)
            dValue = dMin + (int((dValue - dMin) / dInc)) * dInc;

        if ( VmbErrorSuccess != pFeature->SetValue(dValue)) {
            sprintf(msg, "Feature %s set value failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }
        break;
    }

    case VmbFeatureDataEnum:
    {
        std::string sCurrentValue;
        bool bIsAvailable = false;
        pFeature->IsValueAvailable(value.toStdString().c_str(), bIsAvailable );
        if( VmbErrorSuccess != pFeature->IsValueAvailable(value.toStdString().c_str(), bIsAvailable )) {
            sprintf(msg, "Feature %s IsValueAvailable failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        if (!bIsAvailable)
            return -1;

        if( VmbErrorSuccess != pFeature->GetValue(sCurrentValue)) {
            sprintf(msg, "Feature %s GetValue failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        if (sCurrentValue.compare(value.toStdString()) == 0)
            return 0;

        if ( VmbErrorSuccess != pFeature->SetValue(value.toStdString().c_str())) {
            sprintf(msg, "Feature %s set value failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }
        break;
    }

    case VmbFeatureDataString:
    case VmbFeatureDataBool:
    case VmbFeatureDataCommand:
    case VmbFeatureDataRaw:
        break;

    default:
        break;
    }

    return 0;
}

int QVmbCameraclt::GetPara(QString featureName, CameraFeature *pCamearaFeature)
{
    //get feature from featureName
    char msg[1024] = {};
    AVT::VmbAPI::FeaturePtr pFeature;
    VmbErrorType err = VmbErrorSuccess;
    err = SP_ACCESS( pCamera )-> GetFeatureByName( featureName.toLatin1().data(), pFeature );
    if( VmbErrorSuccess != err ) {
        sprintf(msg, "GetFeatureByName failed. errcode = %d.", err);
        logFile(FileLogger::warn, msg);
        return -1;
    }

    //check flags
    VmbFeatureFlagsType flags = VmbFeatureFlagsNone;
    if ((VmbErrorSuccess != pFeature->GetFlags(flags)) ||
            ((VmbFeatureFlagsRead & flags) == 0)) {
        logFile(FileLogger::warn, "Featrue is not readable!");
        return -1;
    }

    //get feature type
    VmbFeatureDataType dataType = VmbFeatureDataUnknown;
    if (VmbErrorSuccess != pFeature->GetDataType(dataType)) {
        logFile(FileLogger::warn, "GetDataType failed!");
        return -1;
    }

    pCamearaFeature->dataType = dataType;

    switch(dataType) {
    case VmbFeatureDataInt:
        break;

    case VmbFeatureDataFloat:
    {
        double dMin = 0.0;
        double dMax = 0.0;
        double dValue = 0.0;
        if( VmbErrorSuccess != pFeature->GetRange(dMin, dMax)) {
            sprintf(msg, "Feature %s GetRange failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        if( VmbErrorSuccess != pFeature->GetValue(dValue)) {
            sprintf(msg, "Feature %s GetValue failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        pCamearaFeature->min = QString::number(dMin);
        pCamearaFeature->max = QString::number(dMax);
        pCamearaFeature->value = QString::number(dValue);

        break;
    }

    case VmbFeatureDataEnum:
    {
        std::vector<std::string>  sEnumEntries;
        std::string sCurrentValue;
        if( VmbErrorSuccess != pFeature->GetValues(sEnumEntries)) {
            sprintf(msg, "Feature %s GetValues failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        if( VmbErrorSuccess != pFeature->GetValue(sCurrentValue)) {
            sprintf(msg, "Feature %s GetValue failed.",
                    featureName.toLatin1().data());
            logFile(FileLogger::warn, msg);
            return -1;
        }

        pCamearaFeature->enumStrings.clear();
        for (int i = 0; i < sEnumEntries.size(); i++) {
            bool bIsAvailable = false;
            pFeature->IsValueAvailable(sEnumEntries.at(i).c_str(), bIsAvailable );

            if(bIsAvailable) {
                pCamearaFeature->enumStrings.append(QString::
                                                    fromStdString(sEnumEntries.at(i)));
            }
        }

        pCamearaFeature->value = QString::fromStdString(sCurrentValue);

        break;
    }

    case VmbFeatureDataString:
    case VmbFeatureDataBool:
    case VmbFeatureDataCommand:
    case VmbFeatureDataRaw:
        break;

    default:
        break;
    }

    return 0;
}

VmbErrorType QVmbCameraclt::CopyToImage(VmbUchar_t *pInBuffer, VmbPixelFormat_t ePixelFormat, QImage &pOutImage, const float *Matrix)
{
    const int           nHeight = camHeight;
    const int           nWidth  = camWidth;
    VmbError_t          Result;

    // Prepare source image
    VmbImage SourceImage;
    SourceImage.Size = sizeof( SourceImage );
    Result = VmbSetImageInfoFromPixelFormat( ePixelFormat, nWidth, nHeight, &SourceImage );
    if ( VmbErrorSuccess != Result )
        return static_cast<VmbErrorType>( Result );

    SourceImage.Data = pInBuffer;

    // Prepare destination image
    VmbImage            DestImage;
    DestImage.Size      = sizeof( DestImage );

    QString             OutputFormat;
    const int           bytes_per_line = pOutImage.bytesPerLine();
    switch( pOutImage.format() ) {
    case QImage::Format_RGB888:
        if( nWidth*3 != bytes_per_line )
            return VmbErrorWrongType;

        OutputFormat = "RGB24";
        break;

    default:
        return VmbErrorBadParameter;
    }

    Result = VmbSetImageInfoFromString( OutputFormat.toStdString().c_str(), OutputFormat.length(),
                                        nWidth,nHeight, &DestImage );
    if ( VmbErrorSuccess != Result )
        return static_cast<VmbErrorType>( Result );

    DestImage.Data      = pOutImage.bits();

    // do color processing?
    if( NULL != Matrix )
    {
        VmbTransformInfo TransformParameter;
        Result = VmbSetColorCorrectionMatrix3x3( Matrix, &TransformParameter );
        if( VmbErrorSuccess == Result )
        {
            Result = VmbImageTransform( &SourceImage, &DestImage, &TransformParameter,1 );
        }
        else
        {
            return static_cast<VmbErrorType>( Result );
        }
    }
    else
    {
        Result = VmbImageTransform( &SourceImage, &DestImage,NULL,0 );
    }
    if( VmbErrorSuccess != Result )
    {
        return static_cast<VmbErrorType>( Result );
    }

    return static_cast<VmbErrorType>( Result );
}

void QVmbCameraclt::OnFrameReady(int status)
{
    // Pick up frame
    AVT::VmbAPI::FramePtr pFrame = SP_DYN_CAST( m_pFrameObserver,
                                                AVT::VmbAPI::FrameObserver )->GetFrame();

    if( SP_ISNULL( pFrame ) ) {
        qDebug() << "frame pointer is NULL, late frame ready message";
        return;
    }

    // See if it is not corrupt
    if ( VmbFrameStatusComplete == status ) {
        VmbUchar_t *pBuffer;
        VmbErrorType err = SP_ACCESS( pFrame )->GetImage( pBuffer );

        if( VmbErrorSuccess == err ) {
            mutex.lock();
            CopyToImage( pBuffer, m_nPixelFormat, curImg );

            // emit signal
            emit hasImage(curImg);

            mutex.unlock();
        }
    }
    else {
        // If we receive an incomplete image we do nothing but logging
        logFile(FileLogger::info, "Failure in receiving image");
    }

    // And queue it to continue streaming
    SP_ACCESS( pCamera )->QueueFrame( pFrame );
}
