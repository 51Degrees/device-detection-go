package ddshared

/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2019 51 Degrees Mobile Experts Limited, 5 Charlotte Close,
 * Caversham, Reading, Berkshire, United Kingdom RG4 7BY.
 *
 * This Original Work is licensed under the European Union Public Licence (EUPL)
 * v.1.2 and is subject to its terms as set out below.
 *
 * If a copy of the EUPL was not distributed with this file, You can obtain
 * one at https://opensource.org/licenses/EUPL-1.2.
 *
 * The 'Compatible Licences' set out in the Appendix to the EUPL (as may be
 * amended by the European Commission) shall be deemed incompatible for
 * the purposes of the Work and the provisions of the compatibility
 * clause in Article 5 of the EUPL shall not apply.
 *
 * If using the Work as, or as part of, a network application, by
 * including the attribution notice(s) required under Article 5 of the EUPL
 * in the end user terms of the application under an appropriate heading,
 * such notice(s) shall fulfill the requirements of that article.
 * ********************************************************************* */
import fiftyone.pipeline.core.data.FlowData;
import fiftyone.pipeline.core.data.types.JavaScript;
import fiftyone.pipeline.engines.data.AspectDataBase;
import fiftyone.pipeline.engines.flowelements.AspectEngine;
import fiftyone.pipeline.engines.services.MissingPropertyService;
import org.slf4j.Logger;

import java.util.List;
public abstract class DeviceDataBase extends AspectDataBase implements DeviceData
{
protected DeviceDataBase(
Logger logger,
FlowData flowData,
AspectEngine engine,
MissingPropertyService missingPropertyService) {
super(logger, flowData, engine, missingPropertyService);
}

public List<String> getUserAgents() { return getAs("UserAgents", List.class); }
public Integer getDifference() { return getAs("Difference", Integer.class); }
public String getDeviceId() { return getAs("DeviceId", String.class); }
public Double getBackCameraMegaPixels() { return getAs("BackCameraMegaPixels", Double.class); }
public Integer getBatteryCapacity() { return getAs("BatteryCapacity", Integer.class); }
public Integer getBitsPerPixel() { return getAs("BitsPerPixel", Integer.class); }
public List getCameraTypes() { return getAs("CameraTypes", List.class); }
public String getContrastRatio() { return getAs("ContrastRatio", String.class); }
public String getCPU() { return getAs("CPU", String.class); }
public Integer getCPUCores() { return getAs("CPUCores", Integer.class); }
public String getCPUDesigner() { return getAs("CPUDesigner", String.class); }
public Double getCPUMaximumFrequency() { return getAs("CPUMaximumFrequency", Double.class); }
public List getDeviceCertifications() { return getAs("DeviceCertifications", List.class); }
public Integer getDeviceRAM() { return getAs("DeviceRAM", Integer.class); }
public String getDeviceType() { return getAs("DeviceType", String.class); }
public List getDurability() { return getAs("Durability", List.class); }
public String getDynamicContrastRatio() { return getAs("DynamicContrastRatio", String.class); }
public Integer getEnergyConsumptionPerYear() { return getAs("EnergyConsumptionPerYear", Integer.class); }
public Integer getExpansionSlotMaxSize() { return getAs("ExpansionSlotMaxSize", Integer.class); }
public List getExpansionSlotType() { return getAs("ExpansionSlotType", List.class); }
public Double getFrontCameraMegaPixels() { return getAs("FrontCameraMegaPixels", Double.class); }
public String getGPU() { return getAs("GPU", String.class); }
public String getGPUDesigner() { return getAs("GPUDesigner", String.class); }
public List getHardwareAudioCodecsDecode() { return getAs("HardwareAudioCodecsDecode", List.class); }
public List getHardwareAudioCodecsEncode() { return getAs("HardwareAudioCodecsEncode", List.class); }
public String getHardwareCarrier() { return getAs("HardwareCarrier", String.class); }
public String getHardwareFamily() { return getAs("HardwareFamily", String.class); }
public List getHardwareImages() { return getAs("HardwareImages", List.class); }
public String getHardwareModel() { return getAs("HardwareModel", String.class); }
public List getHardwareModelVariants() { return getAs("HardwareModelVariants", List.class); }
public List getHardwareName() { return getAs("HardwareName", List.class); }
public Integer getHardwareRank() { return getAs("HardwareRank", Integer.class); }
public String getHardwareVendor() { return getAs("HardwareVendor", String.class); }
public List getHardwareVideoCodecsDecode() { return getAs("HardwareVideoCodecsDecode", List.class); }
public List getHardwareVideoCodecsEncode() { return getAs("HardwareVideoCodecsEncode", List.class); }
public Boolean getHas3DCamera() { return getAs("Has3DCamera", Boolean.class); }
public Boolean getHas3DScreen() { return getAs("Has3DScreen", Boolean.class); }
public Boolean getHasCamera() { return getAs("HasCamera", Boolean.class); }
public Boolean getHasClickWheel() { return getAs("HasClickWheel", Boolean.class); }
public Boolean getHasKeypad() { return getAs("HasKeypad", Boolean.class); }
public Boolean getHasNFC() { return getAs("HasNFC", Boolean.class); }
public Boolean getHasQwertyPad() { return getAs("HasQwertyPad", Boolean.class); }
public Boolean getHasRemovableBattery() { return getAs("HasRemovableBattery", Boolean.class); }
public Boolean getHasTouchScreen() { return getAs("HasTouchScreen", Boolean.class); }
public Boolean getHasTrackPad() { return getAs("HasTrackPad", Boolean.class); }
public Boolean getHasVirtualQwerty() { return getAs("HasVirtualQwerty", Boolean.class); }
public Boolean getIsConsole() { return getAs("IsConsole", Boolean.class); }
public Boolean getIsEReader() { return getAs("IsEReader", Boolean.class); }
public Boolean getIsHardwareGroup() { return getAs("IsHardwareGroup", Boolean.class); }
public Boolean getIsMediaHub() { return getAs("IsMediaHub", Boolean.class); }
public Boolean getIsMobile() { return getAs("IsMobile", Boolean.class); }
public Boolean getIsSmallScreen() { return getAs("IsSmallScreen", Boolean.class); }
public Boolean getIsSmartPhone() { return getAs("IsSmartPhone", Boolean.class); }
public Boolean getIsSmartWatch() { return getAs("IsSmartWatch", Boolean.class); }
public Boolean getIsTablet() { return getAs("IsTablet", Boolean.class); }
public Boolean getIsTv() { return getAs("IsTv", Boolean.class); }
public JavaScript getJavascriptHardwareProfile() { return getAs("JavascriptHardwareProfile", JavaScript.class); }
public Double getMaxInternalStorage() { return getAs("MaxInternalStorage", Double.class); }
public Integer getMaxNumberOfSIMCards() { return getAs("MaxNumberOfSIMCards", Integer.class); }
public Integer getMaxStandbyTime() { return getAs("MaxStandbyTime", Integer.class); }
public Integer getMaxTalkTime() { return getAs("MaxTalkTime", Integer.class); }
public Integer getMaxUsageTime() { return getAs("MaxUsageTime", Integer.class); }
public List getNativeBrand() { return getAs("NativeBrand", List.class); }
public List getNativeDevice() { return getAs("NativeDevice", List.class); }
public List getNativeModel() { return getAs("NativeModel", List.class); }
public List getNativeName() { return getAs("NativeName", List.class); }
@Override
public String getNativePlatform() { return getAs("NativePlatform", String.class); }
public String getOEM() { return getAs("OEM", String.class); }
public Integer getOnPowerConsumption() { return getAs("OnPowerConsumption", Integer.class); }
public String getPopularity() { return getAs("Popularity", String.class); }
public String getPriceBand() { return getAs("PriceBand", String.class); }
public Integer getRefreshRate() { return getAs("RefreshRate", Integer.class); }
public Integer getReleaseAge() { return getAs("ReleaseAge", Integer.class); }
public String getReleaseMonth() { return getAs("ReleaseMonth", String.class); }
public Integer getReleaseYear() { return getAs("ReleaseYear", Integer.class); }
public List getSatelliteNavigationTypes() { return getAs("SatelliteNavigationTypes", List.class); }
public Double getScreenInchesDiagonal() { return getAs("ScreenInchesDiagonal", Double.class); }
public Integer getScreenInchesDiagonalRounded() { return getAs("ScreenInchesDiagonalRounded", Integer.class); }
public Double getScreenInchesHeight() { return getAs("ScreenInchesHeight", Double.class); }
public Integer getScreenInchesSquare() { return getAs("ScreenInchesSquare", Integer.class); }
public Double getScreenInchesWidth() { return getAs("ScreenInchesWidth", Double.class); }
public Double getScreenMMDiagonal() { return getAs("ScreenMMDiagonal", Double.class); }
public Integer getScreenMMDiagonalRounded() { return getAs("ScreenMMDiagonalRounded", Integer.class); }
public Double getScreenMMHeight() { return getAs("ScreenMMHeight", Double.class); }
public Integer getScreenMMSquare() { return getAs("ScreenMMSquare", Integer.class); }
public Double getScreenMMWidth() { return getAs("ScreenMMWidth", Double.class); }
public Integer getScreenPixelsHeight() { return getAs("ScreenPixelsHeight", Integer.class); }
public Integer getScreenPixelsWidth() { return getAs("ScreenPixelsWidth", Integer.class); }
public String getScreenType() { return getAs("ScreenType", String.class); }
public String getSecondBackCameraMegaPixels() { return getAs("SecondBackCameraMegaPixels", String.class); }
public String getSecondFrontCameraMegaPixels() { return getAs("SecondFrontCameraMegaPixels", String.class); }
public String getSoC() { return getAs("SoC", String.class); }
public String getSoCDesigner() { return getAs("SoCDesigner", String.class); }
public String getSoCModel() { return getAs("SoCModel", String.class); }
public String getSpecificAbsorbtionRateEU() { return getAs("SpecificAbsorbtionRateEU", String.class); }
public Integer getSpecificAbsorbtionRateUS() { return getAs("SpecificAbsorbtionRateUS", Integer.class); }
public Double getSuggestedImageButtonHeightMms() { return getAs("SuggestedImageButtonHeightMms", Double.class); }
public Double getSuggestedImageButtonHeightPixels() { return getAs("SuggestedImageButtonHeightPixels", Double.class); }
public Double getSuggestedLinkSizePixels() { return getAs("SuggestedLinkSizePixels", Double.class); }
public Double getSuggestedLinkSizePoints() { return getAs("SuggestedLinkSizePoints", Double.class); }
public List getSupportedBearers() { return getAs("SupportedBearers", List.class); }
public String getSupportedBluetoothVersion() { return getAs("SupportedBluetoothVersion", String.class); }
public List getSupportedCameraFeatures() { return getAs("SupportedCameraFeatures", List.class); }
public List getSupportedChargerTypes() { return getAs("SupportedChargerTypes", List.class); }
public List getSupportedIO() { return getAs("SupportedI/O", List.class); }
public List getSupportedSensorTypes() { return getAs("SupportedSensorTypes", List.class); }
public List getSupportedSIMCardTypes() { return getAs("SupportedSIMCardTypes", List.class); }
public Boolean getSupports24p() { return getAs("Supports24p", Boolean.class); }
public Boolean getSupportsPhoneCalls() { return getAs("SupportsPhoneCalls", Boolean.class); }
public Boolean getSupportsWiDi() { return getAs("SupportsWiDi", Boolean.class); }
public String getWeightWithBattery() { return getAs("WeightWithBattery", String.class); }
public String getWeightWithoutBattery() { return getAs("WeightWithoutBattery", String.class); }
public List getCcppAccept() { return getAs("CcppAccept", List.class); }
public Double getCLDC() { return getAs("CLDC", Double.class); }
public Double getMIDP() { return getAs("MIDP", Double.class); }
public Integer getPlatformDiscontinuedAge() { return getAs("PlatformDiscontinuedAge", Integer.class); }
public String getPlatformDiscontinuedMonth() { return getAs("PlatformDiscontinuedMonth", String.class); }
public String getPlatformDiscontinuedYear() { return getAs("PlatformDiscontinuedYear", String.class); }
public String getPlatformName() { return getAs("PlatformName", String.class); }
public Integer getPlatformPreviewAge() { return getAs("PlatformPreviewAge", Integer.class); }
public String getPlatformPreviewMonth() { return getAs("PlatformPreviewMonth", String.class); }
public String getPlatformPreviewYear() { return getAs("PlatformPreviewYear", String.class); }
public Integer getPlatformRank() { return getAs("PlatformRank", Integer.class); }
public Integer getPlatformReleaseAge() { return getAs("PlatformReleaseAge", Integer.class); }
public String getPlatformReleaseMonth() { return getAs("PlatformReleaseMonth", String.class); }
public String getPlatformReleaseYear() { return getAs("PlatformReleaseYear", String.class); }
public String getPlatformVendor() { return getAs("PlatformVendor", String.class); }
public String getPlatformVersion() { return getAs("PlatformVersion", String.class); }
public List getSoftwareAudioCodecsDecode() { return getAs("SoftwareAudioCodecsDecode", List.class); }
public List getSoftwareAudioCodecsEncode() { return getAs("SoftwareAudioCodecsEncode", List.class); }
public List getSoftwareVideoCodecsDecode() { return getAs("SoftwareVideoCodecsDecode", List.class); }
public List getSoftwareVideoCodecsEncode() { return getAs("SoftwareVideoCodecsEncode", List.class); }
public List getStreamingAccept() { return getAs("StreamingAccept", List.class); }
public String getAjaxRequestType() { return getAs("AjaxRequestType", String.class); }
public Boolean getAnimationTiming() { return getAs("AnimationTiming", Boolean.class); }
public Boolean getBlobBuilder() { return getAs("BlobBuilder", Boolean.class); }
public Integer getBrowserDiscontinuedAge() { return getAs("BrowserDiscontinuedAge", Integer.class); }
public String getBrowserDiscontinuedMonth() { return getAs("BrowserDiscontinuedMonth", String.class); }
public String getBrowserDiscontinuedYear() { return getAs("BrowserDiscontinuedYear", String.class); }
public String getBrowserName() { return getAs("BrowserName", String.class); }
public Integer getBrowserPreviewAge() { return getAs("BrowserPreviewAge", Integer.class); }
public String getBrowserPreviewMonth() { return getAs("BrowserPreviewMonth", String.class); }
public String getBrowserPreviewYear() { return getAs("BrowserPreviewYear", String.class); }
public String getBrowserPropertySource() { return getAs("BrowserPropertySource", String.class); }
public Integer getBrowserRank() { return getAs("BrowserRank", Integer.class); }
public Integer getBrowserReleaseAge() { return getAs("BrowserReleaseAge", Integer.class); }
public String getBrowserReleaseMonth() { return getAs("BrowserReleaseMonth", String.class); }
public Integer getBrowserReleaseYear() { return getAs("BrowserReleaseYear", Integer.class); }
public String getBrowserVendor() { return getAs("BrowserVendor", String.class); }
public String getBrowserVersion() { return getAs("BrowserVersion", String.class); }
public Boolean getCanvas() { return getAs("Canvas", Boolean.class); }
public Boolean getCookiesCapable() { return getAs("CookiesCapable", Boolean.class); }
public Boolean getCssBackground() { return getAs("CssBackground", Boolean.class); }
public Boolean getCssBorderImage() { return getAs("CssBorderImage", Boolean.class); }
public Boolean getCssCanvas() { return getAs("CssCanvas", Boolean.class); }
public Boolean getCssColor() { return getAs("CssColor", Boolean.class); }
public Boolean getCssColumn() { return getAs("CssColumn", Boolean.class); }
public Boolean getCssFlexbox() { return getAs("CssFlexbox", Boolean.class); }
public Boolean getCssFont() { return getAs("CssFont", Boolean.class); }
public Boolean getCssImages() { return getAs("CssImages", Boolean.class); }
public Boolean getCssMediaQueries() { return getAs("CssMediaQueries", Boolean.class); }
public Boolean getCssMinMax() { return getAs("CssMinMax", Boolean.class); }
public Boolean getCssOverflow() { return getAs("CssOverflow", Boolean.class); }
public Boolean getCssPosition() { return getAs("CssPosition", Boolean.class); }
public Boolean getCssText() { return getAs("CssText", Boolean.class); }
public Boolean getCssTransforms() { return getAs("CssTransforms", Boolean.class); }
public Boolean getCssTransitions() { return getAs("CssTransitions", Boolean.class); }
public Boolean getCssUI() { return getAs("CssUI", Boolean.class); }
public Boolean getDataSet() { return getAs("DataSet", Boolean.class); }
public Boolean getDataUrl() { return getAs("DataUrl", Boolean.class); }
public Boolean getDeviceOrientation() { return getAs("DeviceOrientation", Boolean.class); }
public Boolean getFileReader() { return getAs("FileReader", Boolean.class); }
public Boolean getFileSaver() { return getAs("FileSaver", Boolean.class); }
public Boolean getFileWriter() { return getAs("FileWriter", Boolean.class); }
public Boolean getFormData() { return getAs("FormData", Boolean.class); }
public Boolean getFullscreen() { return getAs("Fullscreen", Boolean.class); }
public Boolean getGeoLocation() { return getAs("GeoLocation", Boolean.class); }
public Boolean getHistory() { return getAs("History", Boolean.class); }
public Boolean getHtml5() { return getAs("Html5", Boolean.class); }
public List getHtml5Audio() { return getAs("Html5Audio", List.class); }
public List getHtml5Video() { return getAs("Html5Video", List.class); }
public Boolean getHtmlMediaCapture() { return getAs("Html-Media-Capture", Boolean.class); }
public Double getHtmlVersion() { return getAs("HtmlVersion", Double.class); }
public String getHttpLiveStreaming() { return getAs("HttpLiveStreaming", String.class); }
public Boolean getIframe() { return getAs("Iframe", Boolean.class); }
public Boolean getIndexedDB() { return getAs("IndexedDB", Boolean.class); }
public Boolean getIsDataMinimising() { return getAs("IsDataMinimising", Boolean.class); }
public Boolean getIsEmailBrowser() { return getAs("IsEmailBrowser", Boolean.class); }
public String getIsEmulatingDesktop() { return getAs("IsEmulatingDesktop", String.class); }
public Boolean getIsEmulatingDevice() { return getAs("IsEmulatingDevice", Boolean.class); }
public String getIsWebApp() { return getAs("IsWebApp", String.class); }
public Boolean getJavascript() { return getAs("Javascript", Boolean.class); }
public Boolean getJavascriptCanManipulateCSS() { return getAs("JavascriptCanManipulateCSS", Boolean.class); }
public Boolean getJavascriptCanManipulateDOM() { return getAs("JavascriptCanManipulateDOM", Boolean.class); }
public Boolean getJavascriptGetElementById() { return getAs("JavascriptGetElementById", Boolean.class); }
public String getJavascriptPreferredGeoLocApi() { return getAs("JavascriptPreferredGeoLocApi", String.class); }
public Boolean getJavascriptSupportsEventListener() { return getAs("JavascriptSupportsEventListener", Boolean.class); }
public Boolean getJavascriptSupportsEvents() { return getAs("JavascriptSupportsEvents", Boolean.class); }
public Boolean getJavascriptSupportsInnerHtml() { return getAs("JavascriptSupportsInnerHtml", Boolean.class); }
public Double getJavascriptVersion() { return getAs("JavascriptVersion", Double.class); }
public String getjQueryMobileSupport() { return getAs("jQueryMobileSupport", String.class); }
public Boolean getJson() { return getAs("Json", Boolean.class); }
public String getLayoutEngine() { return getAs("LayoutEngine", String.class); }
public Boolean getMasking() { return getAs("Masking", Boolean.class); }
public Boolean getMeter() { return getAs("Meter", Boolean.class); }
public Boolean getPostMessage() { return getAs("PostMessage", Boolean.class); }
public Boolean getProgress() { return getAs("Progress", Boolean.class); }
public String getPromise() { return getAs("Promise", String.class); }
public Boolean getPrompts() { return getAs("Prompts", Boolean.class); }
public JavaScript getScreenPixelsHeightJavaScript() { return getAs("ScreenPixelsHeightJavaScript", JavaScript.class); }
public JavaScript getScreenPixelsWidthJavaScript() { return getAs("ScreenPixelsWidthJavaScript", JavaScript.class); }
public Boolean getSelector() { return getAs("Selector", Boolean.class); }
public Boolean getSupportsTlsSsl() { return getAs("SupportsTls/Ssl", Boolean.class); }
public Boolean getSupportsWebGL() { return getAs("SupportsWebGL", Boolean.class); }
public Boolean getSvg() { return getAs("Svg", Boolean.class); }
public Boolean getTouchEvents() { return getAs("TouchEvents", Boolean.class); }
public Boolean getTrack() { return getAs("Track", Boolean.class); }
public Boolean getVideo() { return getAs("Video", Boolean.class); }
public Boolean getViewport() { return getAs("Viewport", Boolean.class); }
public Boolean getWebWorkers() { return getAs("WebWorkers", Boolean.class); }
public Boolean getXhr2() { return getAs("Xhr2", Boolean.class); }
public String getCrawlerName() { return getAs("CrawlerName", String.class); }
public Boolean getIsCrawler() { return getAs("IsCrawler", Boolean.class); }
}

