/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_bluetooth_bluetoothcommon_h__
#define mozilla_dom_bluetooth_bluetoothcommon_h__

#include "mozilla/Observer.h"
#include "nsPrintfCString.h"
#include "nsString.h"
#include "nsTArray.h"

extern bool gBluetoothDebugFlag;

#define SWITCH_BT_DEBUG(V) (gBluetoothDebugFlag = V)

#undef BT_LOG
#if defined(MOZ_WIDGET_GONK)
#include <android/log.h>

/**
 * Prints 'D'EBUG build logs, which show in DEBUG build only when
 * developer setting 'Bluetooth output in adb' is enabled.
 */
#define BT_LOGD(msg, ...)                                            \
  do {                                                               \
    if (gBluetoothDebugFlag) {                                       \
      __android_log_print(ANDROID_LOG_INFO, "GeckoBluetooth",        \
                          "%s: " msg, __FUNCTION__, ##__VA_ARGS__);  \
    }                                                                \
  } while(0)

/**
 * Prints 'R'ELEASE build logs, which show in both RELEASE and DEBUG builds.
 */
#define BT_LOGR(msg, ...)                                            \
  __android_log_print(ANDROID_LOG_INFO, "GeckoBluetooth",            \
                      "%s: " msg, __FUNCTION__, ##__VA_ARGS__)       \

/**
 * Prints DEBUG build warnings, which show in DEBUG build only.
 */
#define BT_WARNING(args...)                                          \
  NS_WARNING(nsPrintfCString(args).get())                            \

#else
#define BT_LOGD(msg, ...)                                            \
  do {                                                               \
    if (gBluetoothDebugFlag) {                                       \
      printf("%s: " msg, __FUNCTION__, ##__VA_ARGS__);               \
    }                                                                \
  } while(0)

#define BT_LOGR(msg, ...) printf("%s: " msg, __FUNCTION__, ##__VA_ARGS__))
#define BT_WARNING(msg, ...) printf("%s: " msg, __FUNCTION__, ##__VA_ARGS__))
#endif

/**
 * Prints 'R'ELEASE build logs for WebBluetooth API v2.
 */
#define BT_API2_LOGR(msg, ...)                                       \
  BT_LOGR("[WEBBT-API2] " msg, ##__VA_ARGS__)

/**
 * Wrap literal name and value into a BluetoothNamedValue
 * and append it to the array.
 */
#define BT_APPEND_NAMED_VALUE(array, name, value)                    \
  array.AppendElement(BluetoothNamedValue(NS_LITERAL_STRING(name),   \
                                          BluetoothValue(value)))

/**
 * Ensure success of system message broadcast with void return.
 */
#define BT_ENSURE_TRUE_VOID_BROADCAST_SYSMSG(type, parameters)       \
  do {                                                               \
    if (!BroadcastSystemMessage(type, parameters)) {                 \
      BT_WARNING("Failed to broadcast [%s]",                         \
                 NS_ConvertUTF16toUTF8(type).get());                 \
      return;                                                        \
    }                                                                \
  } while(0)

/**
 * Convert an enum value to string then append it to an array.
 */
#define BT_APPEND_ENUM_STRING(array, enumType, enumValue)            \
  do {                                                               \
    uint32_t index = uint32_t(enumValue);                            \
    nsAutoString name;                                               \
    name.AssignASCII(enumType##Values::strings[index].value,         \
                     enumType##Values::strings[index].length);       \
    array.AppendElement(name);                                       \
  } while(0)                                                         \

/**
 * Resolve promise with |ret| if |x| is false.
 */
#define BT_ENSURE_TRUE_RESOLVE(x, ret)                               \
  do {                                                               \
    if (MOZ_UNLIKELY(!(x))) {                                        \
      BT_API2_LOGR("BT_ENSURE_TRUE_RESOLVE(" #x ") failed");         \
      promise->MaybeResolve(ret);                                    \
      return promise.forget();                                       \
    }                                                                \
  } while(0)

/**
 * Reject promise with |ret| if |x| is false.
 */
#define BT_ENSURE_TRUE_REJECT(x, ret)                                \
  do {                                                               \
    if (MOZ_UNLIKELY(!(x))) {                                        \
      BT_API2_LOGR("BT_ENSURE_TRUE_REJECT(" #x ") failed");          \
      promise->MaybeReject(ret);                                     \
      return promise.forget();                                       \
    }                                                                \
  } while(0)

#define BEGIN_BLUETOOTH_NAMESPACE \
  namespace mozilla { namespace dom { namespace bluetooth {
#define END_BLUETOOTH_NAMESPACE \
  } /* namespace bluetooth */ } /* namespace dom */ } /* namespace mozilla */
#define USING_BLUETOOTH_NAMESPACE \
  using namespace mozilla::dom::bluetooth;

#define KEY_LOCAL_AGENT       "/B2G/bluetooth/agent"
#define KEY_REMOTE_AGENT      "/B2G/bluetooth/remote_device_agent"
#define KEY_MANAGER           "/B2G/bluetooth/manager"
#define KEY_ADAPTER           "/B2G/bluetooth/adapter"

/**
 * When the connection status of a Bluetooth profile is changed, we'll notify
 * observers which register the following topics.
 */
#define BLUETOOTH_A2DP_STATUS_CHANGED_ID "bluetooth-a2dp-status-changed"
#define BLUETOOTH_HFP_STATUS_CHANGED_ID  "bluetooth-hfp-status-changed"
#define BLUETOOTH_HID_STATUS_CHANGED_ID  "bluetooth-hid-status-changed"
#define BLUETOOTH_SCO_STATUS_CHANGED_ID  "bluetooth-sco-status-changed"

/**
 * When the connection status of a Bluetooth profile is changed, we'll
 * dispatch one of the following events.
 */
#define A2DP_STATUS_CHANGED_ID               "a2dpstatuschanged"
#define HFP_STATUS_CHANGED_ID                "hfpstatuschanged"
#define SCO_STATUS_CHANGED_ID                "scostatuschanged"

/**
 * Types of pairing requests for constructing BluetoothPairingEvent and
 * BluetoothPairingHandle.
 */
#define PAIRING_REQ_TYPE_DISPLAYPASSKEY       "displaypasskeyreq"
#define PAIRING_REQ_TYPE_ENTERPINCODE         "enterpincodereq"
#define PAIRING_REQ_TYPE_CONFIRMATION         "pairingconfirmationreq"
#define PAIRING_REQ_TYPE_CONSENT              "pairingconsentreq"

/**
 * When a remote device gets paired / unpaired with local bluetooth adapter,
 * we'll dispatch an event.
 */
#define DEVICE_PAIRED_ID                     "devicepaired"
#define DEVICE_UNPAIRED_ID                   "deviceunpaired"

/**
 * When receiving a query about current play status from remote device, we'll
 * dispatch an event.
 */
#define REQUEST_MEDIA_PLAYSTATUS_ID          "requestmediaplaystatus"

// Bluetooth address format: xx:xx:xx:xx:xx:xx (or xx_xx_xx_xx_xx_xx)
#define BLUETOOTH_ADDRESS_LENGTH 17
#define BLUETOOTH_ADDRESS_NONE   "00:00:00:00:00:00"
#define BLUETOOTH_ADDRESS_BYTES  6

// Bluetooth stack internal error, such as I/O error
#define ERR_INTERNAL_ERROR "InternalError"

BEGIN_BLUETOOTH_NAMESPACE

enum BluetoothStatus {
  STATUS_SUCCESS,
  STATUS_FAIL,
  STATUS_NOT_READY,
  STATUS_NOMEM,
  STATUS_BUSY,
  STATUS_DONE,
  STATUS_UNSUPPORTED,
  STATUS_PARM_INVALID,
  STATUS_UNHANDLED,
  STATUS_AUTH_FAILURE,
  STATUS_RMT_DEV_DOWN
};

enum BluetoothSocketType {
  RFCOMM = 1,
  SCO    = 2,
  L2CAP  = 3,
  EL2CAP = 4
};

enum BluetoothHandsfreeAtResponse {
  HFP_AT_RESPONSE_ERROR,
  HFP_AT_RESPONSE_OK
};

enum BluetoothHandsfreeCallAddressType {
  HFP_CALL_ADDRESS_TYPE_UNKNOWN,
  HFP_CALL_ADDRESS_TYPE_INTERNATIONAL
};

enum BluetoothHandsfreeCallDirection {
  HFP_CALL_DIRECTION_OUTGOING,
  HFP_CALL_DIRECTION_INCOMING
};

enum BluetoothHandsfreeCallMode {
  HFP_CALL_MODE_VOICE,
  HFP_CALL_MODE_DATA,
  HFP_CALL_MODE_FAX
};

enum BluetoothHandsfreeCallMptyType {
  HFP_CALL_MPTY_TYPE_SINGLE,
  HFP_CALL_MPTY_TYPE_MULTI
};

enum BluetoothHandsfreeCallState {
  HFP_CALL_STATE_ACTIVE,
  HFP_CALL_STATE_HELD,
  HFP_CALL_STATE_DIALING,
  HFP_CALL_STATE_ALERTING,
  HFP_CALL_STATE_INCOMING,
  HFP_CALL_STATE_WAITING,
  HFP_CALL_STATE_IDLE
};

enum BluetoothHandsfreeNetworkState {
  HFP_NETWORK_STATE_NOT_AVAILABLE,
  HFP_NETWORK_STATE_AVAILABLE
};

enum BluetoothHandsfreeServiceType {
  HFP_SERVICE_TYPE_HOME,
  HFP_SERVICE_TYPE_ROAMING
};

enum BluetoothHandsfreeVolumeType {
  HFP_VOLUME_TYPE_SPEAKER,
  HFP_VOLUME_TYPE_MICROPHONE
};

class BluetoothSignal;
typedef mozilla::Observer<BluetoothSignal> BluetoothSignalObserver;

// Enums for object types, currently used for shared function lookups
// (get/setproperty, etc...). Possibly discernable via dbus paths, but this
// method is future-proofed for platform independence.
enum BluetoothObjectType {
  TYPE_MANAGER = 0,
  TYPE_ADAPTER = 1,
  TYPE_DEVICE = 2,

  TYPE_INVALID
};

enum ControlPlayStatus {
  PLAYSTATUS_STOPPED  = 0x00,
  PLAYSTATUS_PLAYING  = 0x01,
  PLAYSTATUS_PAUSED   = 0x02,
  PLAYSTATUS_FWD_SEEK = 0x03,
  PLAYSTATUS_REV_SEEK = 0x04,
  PLAYSTATUS_UNKNOWN,
  PLAYSTATUS_ERROR    = 0xFF,
};

enum BluetoothAvrcpPlayerAttribute {
  AVRCP_PLAYER_ATTRIBUTE_EQUALIZER,
  AVRCP_PLAYER_ATTRIBUTE_REPEAT,
  AVRCP_PLAYER_ATTRIBUTE_SHUFFLE,
  AVRCP_PLAYER_ATTRIBUTE_SCAN
};

enum BluetoothAvrcpStatus {
  AVRCP_STATUS_BAD_COMMAND,
  AVRCP_STATUS_BAD_PARAMETER,
  AVRCP_STATUS_NOT_FOUND,
  AVRCP_STATUS_INTERNAL_ERROR,
  AVRCP_STATUS_SUCCESS
};

enum BluetoothAvrcpEvent {
  AVRCP_EVENT_PLAY_STATUS_CHANGED,
  AVRCP_EVENT_TRACK_CHANGE,
  AVRCP_EVENT_TRACK_REACHED_END,
  AVRCP_EVENT_TRACK_REACHED_START,
  AVRCP_EVENT_PLAY_POS_CHANGED,
  AVRCP_EVENT_APP_SETTINGS_CHANGED
};

enum BluetoothAvrcpNotification {
  AVRCP_NTF_INTERIM,
  AVRCP_NTF_CHANGED
};

struct BluetoothAvrcpElementAttribute {
  uint32_t mId;
  nsString mValue;
};

struct BluetoothAvrcpNotificationParam {
  ControlPlayStatus mPlayStatus;
  uint8_t mTrack[8];
  uint32_t mSongPos;
  uint8_t mNumAttr;
  uint8_t mIds[256];
  uint8_t mValues[256];
};

END_BLUETOOTH_NAMESPACE

#endif // mozilla_dom_bluetooth_bluetoothcommon_h__
