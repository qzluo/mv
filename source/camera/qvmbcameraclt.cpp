#include "qvmbcameraclt.h"
#include <QDebug>

QVmbCameraclt::QVmbCameraclt(QObject *parent) : CameraCtl(parent),
    sys( AVT::VmbAPI::VimbaSystem::GetInstance() )
{
    camWidth = 1280;
    camHeight = 960;
}

bool QVmbCameraclt::Initiallize(int DeviceNumber, QString paramPath)
{
    VmbErrorType err = sys.Startup();               // Initialize the Vimba API

    if ( VmbErrorSuccess != err )
        return false;

    AVT::VmbAPI::CameraPtrVector cameras;
    err = sys.GetCameras( cameras );            // Fetch all cameras known to Vimba
    sys.Shutdown();

    if (err != VmbErrorSuccess)
        return false;

    int cameraCount = cameras.size();
    qDebug() << "Cameras found: " << cameras.size() <<"\n\n";

    return (DeviceNumber < cameraCount);
}

bool QVmbCameraclt::StartView()
{
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

    qDebug() << "camWidth: " << camWidth;
    qDebug() << "camHeight: " << camHeight;
    qDebug() << "m_nPixelFormat: " << m_nPixelFormat;

    return true;
}

bool QVmbCameraclt::StopView()
{
    // Stop streaming
    SP_ACCESS( pCamera )->StopContinuousImageAcquisition();

    // Close camera
    pCamera->Close();

    // Clears all remaining frames that have not been picked up
    SP_DYN_CAST( m_pFrameObserver,AVT::VmbAPI::FrameObserver )->
            ClearFrameQueue();

    //关闭系统
    sys.Shutdown();

    return true;
}

Mat QVmbCameraclt::GrabOneFrame()
{
    return Mat();
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

void QVmbCameraclt::OnFrameReady(int status)
{
    qDebug() << "get image";

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
        QImage img = QImage();
        if( VmbErrorSuccess == err ) {
            if (VmbPixelFormatMono8 == m_nPixelFormat) {
                qDebug() << "mono image";
                img = QImage(pBuffer, camWidth,camHeight, QImage::Format_RGB888).
                                    rgbSwapped().copy();
            }
            else {
                qDebug() << "rgb image";
                img = QImage(pBuffer, camWidth,camHeight, QImage::Format_RGB888).
                                    rgbSwapped().copy();
            }

            // emit signal
            emit hasImage(img);
        }
    }
    else {
        // If we receive an incomplete image we do nothing but logging
        qDebug() <<  "Failure in receiving image";
    }

    // And queue it to continue streaming
    SP_ACCESS( pCamera )->QueueFrame( pFrame );
}
