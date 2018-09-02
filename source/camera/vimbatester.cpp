#include "vimbatester.h"
#include <QDebug>
//#include "VimbaCPP/Include/VimbaSystem.h"

#include <VimbaC/Include/VimbaC.h>

void PrintCameraInfo( const AVT::VmbAPI::CameraPtr &camera )
{
    std::string strID;
    std::string strName;
    std::string strModelName;
    std::string strSerialNumber;
    std::string strInterfaceID;

    VmbErrorType err = camera->GetID( strID );
    err = camera->GetName( strName );
    err = camera->GetModel( strModelName );

    err = camera->GetSerialNumber( strSerialNumber );

    err = camera->GetInterfaceID( strInterfaceID );

    qDebug()   << QString("/// Camera Name    : %1").arg(strName.c_str());
    qDebug()   << QString("/// Model Name    : %1").arg(strID.c_str());
    qDebug()   << QString("/// Serial Number   : %1").arg(strSerialNumber.c_str());
    qDebug()   << QString("/// Camera Name    : %1").arg(strInterfaceID.c_str());
}

VimbaTester::VimbaTester()
{

}

void VimbaTester::test()
{
    AVT::VmbAPI::VimbaSystem&    sys = AVT::VmbAPI::VimbaSystem::GetInstance();

    // Get a reference to the VimbaSystem singleton
    qDebug()<<"Vimba Version V"<<"\n";           // Print out version of Vimba
    VmbErrorType    err = sys.Startup();               // Initialize the Vimba API
    AVT::VmbAPI::CameraPtrVector cameras;                           // A vector of std::shared_ptr<AVT::VmbAPI::Camera> objects



    if( VmbErrorSuccess == err )
    {
        err = sys.GetCameras( cameras );            // Fetch all cameras known to Vimba
        if( VmbErrorSuccess == err )
        {
            qDebug() << "Cameras found: " << cameras.size() <<"\n\n";

            // Query all static details of all known cameras and print them out.
            // We don't have to open the cameras for that.
            std::for_each( cameras.begin(), cameras.end(), PrintCameraInfo );
        }
        else
        {
            qDebug() << "Could not list cameras. Error code: " << err;

        }

        sys.Shutdown();                             // Close Vimba
    }
    else
    {
        qDebug() << "Could not start system. Error code: " << err;

    }

}
