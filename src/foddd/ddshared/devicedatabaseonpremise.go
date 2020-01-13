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
import fiftyone.pipeline.core.data.ElementPropertyMetaData;
import fiftyone.pipeline.core.data.FlowData;
import fiftyone.pipeline.core.data.TryGetResult;
import fiftyone.pipeline.core.data.types.JavaScript;
import fiftyone.pipeline.engines.flowelements.AspectEngine;
import fiftyone.pipeline.engines.services.MissingPropertyService;
import org.slf4j.Logger;

import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import static fiftyone.pipeline.util.Types.getPrimativeTypeMap;

public abstract class DeviceDataBaseOnPremise extends DeviceDataBase {

protected final Map<Class<?>, Class<?>> primativeTypes;
private Object dataLock = new Object();
private Object getLock = new Object();
private boolean mapPopulated = false;

protected DeviceDataBaseOnPremise(
Logger logger,
FlowData flowData,
AspectEngine engine,
MissingPropertyService missingPropertyService) {
super(logger, flowData, engine, missingPropertyService);
primativeTypes = getPrimativeTypeMap();
}

protected abstract boolean propertyIsAvailable(String propertyName);

public abstract List<String> getValues(String propertyName);

protected abstract String getValueAsString(String propertyName);

protected abstract Integer getValueAsNullableInteger(String propertyName);

protected abstract Boolean getValueAsNullableBool(String propertyName);

protected abstract Double getValueAsNullableDouble(String propertyName);

protected abstract int getValueAsInteger(String propertyName);

protected abstract boolean getValueAsBool(String propertyName);

protected abstract double getValueAsDouble(String propertyName);

@Override
public Map<String, Object> asKeyMap() {
if (mapPopulated == false) {
synchronized (dataLock) {
if (mapPopulated == false) {
Map<String, Object> map = new TreeMap<>(
String.CASE_INSENSITIVE_ORDER);
for (ElementPropertyMetaData property : getPipline()
.getElementAvailableProperties()
.get(getEngines().get(0).getElementDataKey()).values()) {
map.put(property.getName().toLowerCase(),
getAs(property.getName(), property.getType()));
}
populateFromMap(map);
mapPopulated = true;
}
}
}
// Now that the base map has been populated,
// we can return it.
return super.asKeyMap();
}

protected Class getPropertyType(String propertyName) {
Class type = Object.class;
Map<String, ElementPropertyMetaData> properties =
getPipline().getElementAvailableProperties()
.get(getEngines().get(0).getElementDataKey());
if (properties != null) {
ElementPropertyMetaData property = properties.get(propertyName);
if (property != null) {
type = property.getType();
}
}
return type;
}

@Override
protected <T> TryGetResult<T> tryGetValue(String key, Class<T> type) {
TryGetResult<T> result = new TryGetResult<>();
if (mapPopulated == true) {
// If the complete set of values has been populated
// then we can use the base implementation to get
// the value from the dictionary.
return super.tryGetValue(key, type);
} else {
// If the complete set of values has not been populated
// then we don't want to retrieve values for all
// properties so just get the one we want.
if (propertyIsAvailable(key)) {
if (type.equals(Object.class)) {
type = getPropertyType(key);
}
synchronized (getLock) {
Object obj = null;
if (type.equals(String.class)) {
obj = getValueAsString(key);
} else if (type.equals(Double.class)) {
obj = getValueAsNullableDouble(key);
} else if (type.equals(Integer.class)) {
obj = getValueAsNullableInteger(key);
} else if (type.equals(Boolean.class)) {
obj = getValueAsNullableBool(key);
} else if (type.equals(boolean.class)) {
obj = getValueAsBool(key);
} else if (type.equals(int.class)) {
obj = getValueAsInteger(key);
} else if (type.equals(double.class)) {
obj = getValueAsDouble(key);
} else if (type.equals(List.class)) {
obj = getValues(key);
} else if (type.equals(JavaScript.class)) {
obj = new JavaScript(getValueAsString(key));
} else if (type.equals(Object.class)) {
obj = getValues(key);
}
try {
T value;
if (type.isPrimitive()) {
value = (T) primativeTypes.get(type).cast(obj);
} else {
value = type.cast(obj);
}
result.setValue(value);
} catch (ClassCastException e) {
throw new ClassCastException(
"Expected property '" + key + "' to be of " +
"type '" + type.getSimpleName() + "' but it is " +
"'" + obj.getClass().getSimpleName() + "'");
}
}
}
}
return result;
}

}
