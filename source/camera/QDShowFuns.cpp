#include "QDShowFuns.h"

#include <QDebug>

#pragma comment(lib,"Strmiids.lib")

QDShowFuns::QDShowFuns()
{

}

IMoniker* QDShowFuns::getCameraMoniker(int nCamID)
{
    if (nCamID < 0)
        return NULL;

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
                                  CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
                                  (void**)&pCreateDevEnum);

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR)
        return NULL;

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    int count = 0;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK) {
        if (count == nCamID)
            return pM;

        count++;
    }

    pCreateDevEnum = NULL;
    pEm = NULL;

    return NULL;
}

IBaseFilter *QDShowFuns::getSoureFilter(int nCamID)
{
    IMoniker *pM = getCameraMoniker(nCamID);
    if (!pM)
        return NULL;

    LPOLESTR strName = NULL;
    HRESULT hr = pM->GetDisplayName(NULL, NULL, &strName);
    if (!SUCCEEDED(hr)) {
        pM->Release();
        return NULL;
    }

    //create device moniker
    IBindCtx *pBindCtx;
    hr = CreateBindCtx(0, &pBindCtx);
    if (!SUCCEEDED(hr)) {
        pM->Release();
        return NULL;
    }

    ULONG chEaten = 0;
    IMoniker *pMoniker = 0;
    hr = MkParseDisplayName(pBindCtx, strName, &chEaten, &pMoniker);
    if (!SUCCEEDED(hr)) {
        pBindCtx->Release();
        pM->Release();
        return NULL;
    }

    CoTaskMemFree(strName);

    IBaseFilter *pFilter = NULL;
    hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pFilter);
    if (!SUCCEEDED(hr)) {
        pMoniker->Release();
        pBindCtx->Release();
        pM->Release();
        return NULL;
    }

    pBindCtx->Release();
    pMoniker->Release();
    pM->Release();

    return pFilter;
}

///
/// \brief QDShowFuns::getCameraCount
/// \return  >=0 -- camera count, -1 -- failed
///
int QDShowFuns::getCameraCount()
{
    int count = 0;
    CoInitialize(NULL);

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
                                  CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
                                  (void**)&pCreateDevEnum);

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR) {
        CoUninitialize();
        return 0;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK) {
        count++;
    }

    pM->Release();
    pCreateDevEnum = NULL;
    pEm = NULL;

    CoUninitialize();

    return count;
}

///
/// \brief QDShowFuns::getCameraName
/// \param nCamID
/// \param sName
/// \param nBufferSize
/// \return  0 -- success, -1 -- failed
///
int QDShowFuns::getCameraName(int nCamID, char *sName, int nBufferSize)
{
    CoInitialize(NULL);

    IMoniker *pM = getCameraMoniker(nCamID);
    if (!pM) {
        CoUninitialize();
        return -1;
    }

    IPropertyBag *pBag = 0;
    HRESULT hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
    if (SUCCEEDED(hr)) {
        VARIANT var;
        var.vt = VT_BSTR;
        hr = pBag->Read(L"FriendlyName", &var, NULL);
        if (hr == NOERROR) {
            //获取设备名称
            WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1,
                                sName, nBufferSize, "", NULL);
            SysFreeString(var.bstrVal);
        }
        pBag->Release();
    }

    pM->Release();
    CoUninitialize();

    return 0;
}

///
/// \brief QDShowFuns::getDeviceResolusions
/// \param nCamId
/// \param widths
/// \param heights
/// \return 0 -- success, -1 -- failed
///
int QDShowFuns::getDeviceResolusions(int nCamId, QVector<int> &widths, QVector<int> &heights)
{
    CoInitialize(NULL);

    IMoniker *pMoniker = getCameraMoniker(nCamId);
    if (!pMoniker) {
        CoUninitialize();
        return -1;
    }

    HRESULT hr = NULL;
    IBaseFilter *pFilter;
    hr = pMoniker->BindToObject(NULL, NULL,
                                IID_IBaseFilter,
                                (void**)&pFilter);
    if (!pFilter) {
        CoUninitialize();
        return -1;
    }

    IEnumPins * pinEnum = NULL;
    IPin * pin = NULL;
    if (FAILED(pFilter->EnumPins(&pinEnum))) {
        pinEnum->Release();
        CoUninitialize();
        return -1;
    }

    pinEnum->Reset();
    ULONG pinFetched = 0;
    while (SUCCEEDED(pinEnum->Next(1, &pin, &pinFetched)) && pinFetched) {
        if (!pin)
            continue;

        PIN_INFO pinInfo;
        if (FAILED(pin->QueryPinInfo(&pinInfo)))
            continue;

        if (pinInfo.dir != PINDIR_OUTPUT)
            continue;

        IEnumMediaTypes *mtEnum = NULL;
        AM_MEDIA_TYPE   *mt = NULL;
        if (FAILED(pin->EnumMediaTypes(&mtEnum)))
            break;

        mtEnum->Reset();
        ULONG mtFetched = 0;
        while (SUCCEEDED(mtEnum->Next(1, &mt, &mtFetched)) && mtFetched) {
            BITMAPINFOHEADER* bmi = NULL;
            if (mt->formattype == FORMAT_VideoInfo) {
                if ( mt->cbFormat >= sizeof(VIDEOINFOHEADER)){
                    VIDEOINFOHEADER *pVih = reinterpret_cast<VIDEOINFOHEADER*>( mt->pbFormat);
                    bmi = &( pVih->bmiHeader );;
                }
            }
            if( bmi ){
                widths.push_back(bmi->biWidth);
                heights.push_back(bmi->biHeight);
            }
        }

        mtEnum->Release();
        pin->Release();
    }

    pinEnum->Release();
    pMoniker->Release();

    CoUninitialize();

    return 0;
}

int QDShowFuns::getDeviceMaxResolusion(int nCamId, int& width, int& height)
{
    QVector<int> widths;
    QVector<int> heights;
    int ret = getDeviceResolusions(nCamId, widths, heights);
    if (ret < 0)
        return -1;

    if (widths.size() == 0)
        return -1;

    Q_ASSERT(widths.size() == heights.size());

    //get max width
    width = widths[0];
    height = heights[0];
    for (int i = 0; i < widths.size(); ++i) {
        if (width < widths[i]) {
            width = widths[i];
            height = heights[i];
        }
    }

    return 0;
}

void QDShowFuns::showPropertyPage(int nCamID)
{
    CoInitialize(NULL);

    IBaseFilter *pFilter = getSoureFilter(nCamID);
    if (!pFilter) {
        CoUninitialize();
        return ;
    }

    //show property page
    ISpecifyPropertyPages *pSpec = NULL;
    CAUUID cauuid;

    HRESULT hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages,
                                 (void **)&pSpec);
    if (hr == S_OK)
    {
        hr = pSpec->GetPages(&cauuid);

        hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
                                    (IUnknown **)&pFilter, cauuid.cElems,
                                    (GUID *)cauuid.pElems, 0, 0, NULL);

        CoTaskMemFree(cauuid.pElems);
        pSpec->Release();
    }

    pFilter->Release();
    CoUninitialize();

    return ;
}

int QDShowFuns::getBrightness(IBaseFilter *capFilter, int* pBrightness)
{
    long min = 0, max = 0, def = 0, step = 0, text = 0;
    long iBrightness = 0;
    IAMVideoProcAmp  *pVideoProcAmp = NULL;

    HRESULT hr = capFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&pVideoProcAmp);
    if (pVideoProcAmp != NULL)
    {
        if (S_OK == (hr = pVideoProcAmp->GetRange(VideoProcAmp_Brightness, &min, &max, &step, &def, &text)))
        {
            hr = pVideoProcAmp->Get(VideoProcAmp_Brightness, &iBrightness, &text);
            if (SUCCEEDED(hr)) {
                *pBrightness = iBrightness;
                return 0;
            }
        }
    }

    return -1;

//    IAMCameraControl  *pCameraControl = NULL;

//    HRESULT hr = capFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
//    if (pCameraControl != NULL)
//    {
//        if (S_OK == (hr = pCameraControl->GetRange(CameraControl_Exposure, &min, &max, &step, &def, &text)))
//        {
//            hr = pCameraControl->Get(CameraControl_Exposure, &iExposure, &text);
//        }
//    }
}

int QDShowFuns::setBrightness(IBaseFilter *capFilter, int iBrightness)
{
    long min = 0, max = 0, def = 0, step = 0, text = 0;
    IAMVideoProcAmp *pVideoProcAmp = NULL;
    HRESULT hr = capFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&pVideoProcAmp);
    if (pVideoProcAmp != NULL)
    {
        if (S_OK == (hr = pVideoProcAmp->GetRange(VideoProcAmp_Brightness, &min, &max, &step, &def, &text)))
        {
            hr = pVideoProcAmp->Set(VideoProcAmp_Brightness, iBrightness, text);
            if (SUCCEEDED(hr))
                return 0;
        }
    }

    return -1;
}

int QDShowFuns::getBrightnessRange(IBaseFilter *capFilter, int *pMin, int *pMax)
{
    long min = 0, max = 0, def = 0, step = 0, text = 0;
    IAMVideoProcAmp  *pVideoProcAmp = NULL;

    HRESULT hr = capFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&pVideoProcAmp);
    if (pVideoProcAmp != NULL) {
        hr = pVideoProcAmp->GetRange(VideoProcAmp_Brightness,
                                     &min, &max, &step, &def, &text);
        if (S_OK == hr) {
            *pMin = min;
            *pMax = max;
            return 0;
        }
    }

    return -1;
}

int QDShowFuns::getBrightness(int nCamID, int* pBrightness)
{
    CoInitialize(NULL);

    IBaseFilter *pFilter = getSoureFilter(nCamID);
    if (!pFilter) {
        CoUninitialize();
        return -1;
    }

    int ret = getBrightness(pFilter, pBrightness);

    CoUninitialize();

    return ret;
}

int QDShowFuns::setBrightness(int nCamID, int iBrightness)
{
    CoInitialize(NULL);

    IBaseFilter *pFilter = getSoureFilter(nCamID);
    if (!pFilter) {
        CoUninitialize();
        return -1;
    }

    int ret = setBrightness(pFilter, iBrightness);

    CoUninitialize();

    return ret;
}

int QDShowFuns::getBrightnessRange(int nCamID, int *pMin, int *pMax)
{
    CoInitialize(NULL);

    IBaseFilter *pFilter = getSoureFilter(nCamID);
    if (!pFilter) {
        CoUninitialize();
        return -1;
    }

    int ret = getBrightnessRange(pFilter, pMin, pMax);

    CoUninitialize();

    return ret;
}
