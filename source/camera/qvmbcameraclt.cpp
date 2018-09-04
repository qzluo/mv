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
    sys.Shutdown();

    if (err != VmbErrorSuccess) {
        logFile(FileLogger::warn, "Failed to get cameras.");
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
        return false;
    }

    return true;
}

bool QVmbCameraclt::StartView()
{
    bStarted = false;

    //启动系统
    VmbErrorType err = sys.Startup();               // Initialize the Vimba API
    if ( VmbErrorSuccess != err )
        return false;

    //打开相机
    AVT::VmbAPI::CameraPtrVector cameras;
    err = sys.GetCameras( cameras );
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
    pCamera->Close();

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

void QVmbCameraclt::SetPara()
{

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
    if( VmbFrameStatusComplete == status ) {
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
