package ddshared

/*******************************************************************************
 * This Source Code Form is copyright of 51Degrees Mobile Experts Limited.
 * Copyright 2017 51Degrees Mobile Experts Limited, 5 Charlotte Close,
 * Caversham, Reading, Berkshire, United Kingdom RG4 7BY
 *
 * This Source Code Form is the subject of the following patents and patent
 * applications, owned by 51Degrees Mobile Experts Limited of 5 Charlotte
 * Close, Caversham, Reading, Berkshire, United Kingdom RG4 7BY:
 * European Patent No. 2871816;
 * European Patent Application No. 17184134.9;
 * United States Patent Nos. 9,332,086 and 9,350,823; and
 * United States Patent Application No. 15/686,066.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.
 *
 * If a copy of the MPL was not distributed with this file, You can obtain
 * one at http://mozilla.org/MPL/2.0/.
 *
 * This Source Code Form is "Incompatible With Secondary Licenses", as
 * defined by the Mozilla Public License, v. 2.0.
 ******************************************************************************/
import fiftyone.devicedetection.cloud.flowelements.DeviceDetectionCloudEngineBuilder;
import fiftyone.pipeline.cloudrequestengine.flowelements.CloudRequestEngine;
import fiftyone.pipeline.cloudrequestengine.flowelements.CloudRequestEngineBuilder;
import fiftyone.pipeline.core.flowelements.Pipeline;
import fiftyone.pipeline.engines.configuration.CacheConfiguration;
import fiftyone.pipeline.engines.configuration.LazyLoadingConfiguration;
import fiftyone.pipeline.engines.flowelements.CloudPipelineBuilderBase;
import fiftyone.pipeline.engines.services.HttpClient;
import org.slf4j.ILoggerFactory;

public class DeviceDetectionCloudPipelineBuilder
extends CloudPipelineBuilderBase<DeviceDetectionCloudPipelineBuilder> {

private final HttpClient httpClient;

public DeviceDetectionCloudPipelineBuilder(
ILoggerFactory loggerFactory,
HttpClient httpClient) {
super(loggerFactory);
this.httpClient = httpClient;
}

@Override
public Pipeline build() throws Exception {
// Build and build the cloud request engine
CloudRequestEngineBuilder cloudRequestEngineBuilder =
new CloudRequestEngineBuilder(loggerFactory, httpClient);
if (lazyLoading) {
cloudRequestEngineBuilder.setLazyLoading(new LazyLoadingConfiguration(
(int) lazyLoadingTimeoutMillis));
}
if (resultsCache) {
cloudRequestEngineBuilder.setCache(
new CacheConfiguration(resultsCacheSize));
}
if (url != null && url.isEmpty() == false) {
cloudRequestEngineBuilder.setDataEndpoint(url);
}
if (propertiesEndpoint != null && propertiesEndpoint.isEmpty() == false) {
cloudRequestEngineBuilder.setPropertiesEndpoint(propertiesEndpoint);
}
if (resourceKey != null && resourceKey.isEmpty() == false) {
cloudRequestEngineBuilder.setResourceKey(resourceKey);
}
if (licenseKey != null && licenseKey.isEmpty() == false) {
cloudRequestEngineBuilder.setLicenseKey(licenseKey);
}
CloudRequestEngine cloudRequestEngine = cloudRequestEngineBuilder.build();

// Build and build the device detection engine
DeviceDetectionCloudEngineBuilder deviceDetectionEngineBuilder =
new DeviceDetectionCloudEngineBuilder(
loggerFactory,
httpClient,
cloudRequestEngine);
if (lazyLoading) {
deviceDetectionEngineBuilder.setLazyLoading(new LazyLoadingConfiguration(
(int) lazyLoadingTimeoutMillis));
}

// Add the elements to the list
flowElements.add(cloudRequestEngine);
flowElements.add(deviceDetectionEngineBuilder.build());

// Build and return the pipeline
return super.build();
}
}

