/**
 * Created January 12, 2021
 * 
 * This work is a part of Firebase ESP Client library
 * Copyright (c) 2021, 2021 K. Suwatchai (Mobizt)
 * 
 * The MIT License (MIT)
 * Copyright (c) 2021, 2021 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FB_COMMON_H_
#define FB_COMMON_H_

#include <Arduino.h>
#include <SPI.h>
#include <time.h>
#include <SD.h>
#include <vector>
#include <functional>
#if defined(ESP32)
#include <WiFi.h>
#include <SPIFFS.h>
#include "wcs/esp32/FB_HTTPClient32.h"
#elif defined(ESP8266)
#include <Schedule.h>
#include <ets_sys.h>
#include <ESP8266WiFi.h>
#include "wcs/esp8266/FB_HTTPClient.h"
#define FS_NO_GLOBALS
#endif

#include <FS.h>
#include "json/FirebaseJson.h"

class FirebaseData;

#define FIREBASE_PORT 443
#define KEEP_ALIVE_TIMEOUT 45000
#define STREAM_ERROR_NOTIFIED_INTERVAL 3000
#define STREAM_RECONNECT_INTERVAL 1000
#define SD_CS_PIN 15
#define MAX_REDIRECT 5
#define WIFI_RECONNECT_TIMEOUT 10000
#define STREAM_TASK_STACK_SIZE 8192
#define QUEUE_TASK_STACK_SIZE 8192
#define MAX_BLOB_PAYLOAD_SIZE 1024
#define MAX_EXCHANGE_TOKEN_ATTEMPTS 5

    enum fb_esp_fcm_msg_mode
{
    fb_esp_fcm_msg_mode_legacy_http,
    fb_esp_fcm_msg_mode_httpv1,
    fb_esp_fcm_msg_mode_subscribe,
    fb_esp_fcm_msg_mode_unsubscribe,
    fb_esp_fcm_msg_mode_app_instance_info,
    fb_esp_fcm_msg_mode_apn_token_registration
};

enum fb_esp_mem_storage_type
{
    mem_storage_type_undefined,
    mem_storage_type_flash,
    mem_storage_type_sd
};

enum fb_esp_con_mode
{
    fb_esp_con_mode_undefined,
    fb_esp_con_mode_rtdb,
    fb_esp_con_mode_rtdb_stream,
    fb_esp_con_mode_fcm,
    fb_esp_con_mode_storage
};

enum fb_esp_data_type
{
    d_any,
    d_null,
    d_integer,
    d_float,
    d_double,
    d_boolean,
    d_string,
    d_json,
    d_array,
    d_blob,
    d_file,
    d_timestamp,
    d_shallow,
};

enum fb_esp_method
{
    m_put,
    m_put_nocontent,
    m_post,
    m_get,
    m_get_nocontent,
    m_stream,
    m_patch,
    m_patch_nocontent,
    m_delete,
    m_download,
    m_restore,
    m_read_rules,
    m_set_rules,
    m_get_shallow,
    m_get_priority,
    m_set_priority,
};

enum fb_esp_settings_provider_type
{
    auth_provider_type_login,
    auth_provider_type_service_account
};

enum fb_esp_auth_token_status
{
    token_status_uninitialized,
    token_status_on_signing,
    token_status_on_request,
    token_status_on_refresh,
    token_status_ready,
    token_status_error
};

enum fb_esp_auth_token_type
{
    token_type_undefined,
    token_type_legacy_token,
    token_type_id_token,
    token_type_custom_token,
    token_type_oauth2_access_token
};

enum fb_esp_jwt_generation_step
{
    fb_esp_jwt_generation_step_begin,
    fb_esp_jwt_generation_step_encode_header_payload,
    fb_esp_jwt_generation_step_sign,
    fb_esp_jwt_generation_step_exchange
};

enum fb_esp_user_email_sending_type
{
    fb_esp_user_email_sending_type_verify,
    fb_esp_user_email_sending_type_reset_psw
};

enum fb_esp_gcs_request_type
{
    fb_esp_gcs_request_type_undefined,
    fb_esp_gcs_request_type_upload_simple,
    fb_esp_gcs_request_type_upload_multipart,
    fb_esp_gcs_request_type_upload_resumable_init,
    fb_esp_gcs_request_type_upload_resumable_run,
    fb_esp_gcs_request_type_download,
    fb_esp_gcs_request_type_patch,
    fb_esp_gcs_request_type_delete,
    fb_esp_gcs_request_type_get_metadata,
    fb_esp_gcs_request_type_set_metadata,
    fb_esp_gcs_request_type_update_metadata,
    fb_esp_gcs_request_type_list
};

enum fb_esp_fcs_request_type
{
    fb_esp_fcs_request_type_undefined,
    fb_esp_fcs_request_type_upload,
    fb_esp_fcs_request_type_download,
    fb_esp_fcs_request_type_get_meta,
    fb_esp_fcs_request_type_delete,
    fb_esp_fcs_request_type_list
};

struct fb_esp_rtdb_request_info_t
{
    std::string path = "";
    fb_esp_method method = m_get;
    fb_esp_data_type dataType = d_any;
    std::string payload = "";
    std::string priority = "";
    std::string etag = "";
    bool queue = false;
    std::string filename = "";
    fb_esp_mem_storage_type storageType = mem_storage_type_undefined;
};

struct fb_esp_rtdb_queue_info_t
{
    fb_esp_method method = m_get;
    fb_esp_mem_storage_type storageType = mem_storage_type_undefined;
    fb_esp_data_type dataType;
    std::string path = "";
    std::string filename = "";
    std::string payload = "";
    bool isQuery = false;
    int *intTarget = nullptr;
    float *floatTarget = nullptr;
    double *doubleTarget = nullptr;
    bool *boolTarget = nullptr;
    String *stringTarget = nullptr;
    std::vector<uint8_t> *blobTarget = nullptr;
    FirebaseJson *jsonTarget = nullptr;
    FirebaseJsonArray *arrTarget = nullptr;
};

struct server_response_data_t
{
    int httpCode = -1;
    int payloadLen = -1;
    int contentLen = -1;
    int chunkRange = 0;
    fb_esp_data_type dataType = fb_esp_data_type::d_any;
    int payloadOfs = 0;
    bool boolData = false;
    int intData = 0;
    float floatData = 0.0f;
    double doubleData = 0.0f;
    std::vector<uint8_t> blobData;
    bool isEvent = false;
    bool noEvent = false;
    bool isChunkedEnc = false;
    bool hasEventData = false;
    bool noContent = false;
    bool eventPathChanged = false;
    bool dataChanged = false;
    std::string location = "";
    std::string contentType = "";
    std::string connection = "";
    std::string eventPath = "";
    std::string eventType = "";
    std::string eventData = "";
    std::string etag = "";
    std::string pushName = "";
    std::string fbError = "";
    std::string transferEnc = "";
};

struct fb_esp_auth_token_error_t
{
    std::string message;
    int code = 0;
};

struct fb_esp_auth_token_info_t
{
    std::string id_token;
    std::string refresh_token;
    std::string access_token;
    std::string legacy_token;
    std::string auth_type;
    std::string jwt;
    unsigned long expires = 0;
    fb_esp_auth_token_type token_type = token_type_undefined;
    fb_esp_auth_token_status status = token_status_uninitialized;
    struct fb_esp_auth_token_error_t error;
};

struct fb_esp_auth_signin_token_t
{
    std::string uid;
    FirebaseJson claims;
};

struct fb_esp_service_account_data_info_t
{
    std::string client_email;
    std::string client_id;
    std::string project_id;
    std::string private_key_id;
    const char *private_key = "";
};

struct fb_esp_auth_signin_user_t
{
    std::string email;
    std::string password;
};

struct fb_esp_auth_cert_t
{
    const char *data = "";
    std::string file;
    fb_esp_mem_storage_type file_storage = mem_storage_type_flash;
};

struct fb_esp_service_account_file_info_t
{
    std::string path;
    fb_esp_mem_storage_type storage_type = mem_storage_type_flash;
};

struct fb_esp_service_account_t
{
    struct fb_esp_service_account_data_info_t data;
    struct fb_esp_service_account_file_info_t json;
};

struct fb_esp_auth_signin_provider_t
{
    struct fb_esp_auth_signin_user_t user;
    struct fb_esp_auth_signin_token_t token;
};

struct fb_esp_token_signer_resources_t
{
    int step = 0;
    int attempts = 0;
    bool signup = false;
    unsigned long lastReqMillis = 0;
    unsigned long preRefreshMillis = 5 * 60 * 1000;
    unsigned long reqTO = 2000;
    std::string pk;
    size_t hashSize = 32; //SHA256 size (256 bits or 32 bytes)
    size_t signatureSize = 256;
#if defined(ESP32)
    uint8_t *hash = nullptr;
#elif defined(ESP8266)
    char *hash = nullptr;
#endif
    unsigned char *signature = nullptr;
    std::string header;
    std::string payload;
    std::string encHeader;
    std::string encPayload;
    std::string encHeadPayload;
    std::string encSignature;
#if defined(ESP32)
    mbedtls_pk_context *pk_ctx = nullptr;
    mbedtls_entropy_context *entropy_ctx = nullptr;
    mbedtls_ctr_drbg_context *ctr_drbg_ctx = nullptr;
    FB_HTTPClient32 *wcs = nullptr;
#elif defined(ESP8266)
    WiFiClientSecure *wcs = nullptr;
#endif
    FirebaseJson *json = nullptr;
    FirebaseJsonData *data = nullptr;
    struct fb_esp_auth_token_info_t tokens;
    struct fb_esp_auth_token_error_t verificationError;
    struct fb_esp_auth_token_error_t resetPswError;
    struct fb_esp_auth_token_error_t signupError;
};

struct fb_esp_stream_info_t
{
    std::string stream_path = "";
    std::string path = "";
    std::string data = "";
    std::vector<uint8_t> blob = std::vector<uint8_t>();
    std::string data_type_str = "";
    std::string event_type_str = "";
    uint8_t data_type = 0;
    int idx = -1;

    fb_esp_data_type m_type = d_any;
    std::string m_data = "";
    std::string m_path = "";
    std::string m_type_str = "";
    FirebaseJson *m_json = nullptr;
};

struct fb_esp_cfg_int_t
{
    bool fb_multiple_requests = false;
    bool fb_processing  = false;
    uint8_t fb_stream_idx  = 0;
    fs::File fb_file;
    bool fb_sd_rdy = false;
    bool fb_flash_rdy  = false;
     bool fb_sd_used  = false;
     bool fb_reconnect_wifi  = false;
     unsigned long fb_last_reconnect_millis  = 0;
     uint16_t fb_reconnect_tmo  = WIFI_RECONNECT_TIMEOUT;
     bool fb_clock_rdy  = false;
     float fb_gmt_offset  = 0;
     std::shared_ptr<const char> fb_caCert = nullptr;
     uint8_t fb_float_digits  = 5;
     uint8_t fb_double_digits  = 9;
     bool fb_auth_uri  = false;
     std::vector<std::reference_wrapper<FirebaseData>> fb_sdo;
};

struct fb_esp_cfg_t
{
    struct fb_esp_service_account_t service_account;
    std::string host;
    std::string api_key;
    float time_zone = 0;
    struct fb_esp_auth_cert_t cert;
    struct fb_esp_token_signer_resources_t signer;
    struct fb_esp_cfg_int_t _int;
};

struct fb_esp_url_info_t
{
    std::string host;
    std::string uri;
    std::string auth;
};

struct fb_esp_fcs_file_list_item_t
{
    std::string name;
    std::string bucket;
};

struct fb_esp_fcs_file_list_t
{
    std::vector<struct fb_esp_fcs_file_list_item_t> items = std::vector<struct fb_esp_fcs_file_list_item_t>();
};

struct token_info_t
{
    fb_esp_auth_token_type type = token_type_undefined;
    fb_esp_auth_token_status status = token_status_uninitialized;
    struct fb_esp_auth_token_error_t error;
};

struct fb_esp_rtdb_info_t
{
    int queue_Idx = -1;
    int Idx = -1;
    uint8_t data_type = 0;
    bool data_tmo = false;
    bool no_content_req = false;
    bool stream_data_changed = false;
    bool stream_path_changed = false;
    bool data_available = false;
    bool keep_alive = false;
    bool data_mismatch = false;
    bool path_not_found = false;
    bool pause = false;
    bool stream_stop = false;

    uint8_t connection_status = 0;
    uint32_t queue_ID = 0;
    uint8_t max_retry = 0;

    fb_esp_method req_method = fb_esp_method::m_put;
    fb_esp_data_type req_data_type = fb_esp_data_type::d_any;
    fb_esp_data_type resp_data_type = fb_esp_data_type::d_any;

    std::string path = "";
    std::string data = "";
    std::string data2 = "";
    std::string stream_path = "";
    std::string push_name = "";
    std::string file_name = "";
    std::string redirect_url = "";
    std::string event_type = "";
    std::string req_etag = "";
    std::string resp_etag = "";
    std::string priority = "";
    fb_esp_mem_storage_type storage_type = mem_storage_type_flash;
    uint8_t redirect_count = 0;
    int redirect = 0;

    uint16_t max_blob_size = MAX_BLOB_PAYLOAD_SIZE;

    unsigned long stream_tmo_Millis = 0;
    unsigned long stream_resume_millis = 0;
    unsigned long data_millis = 0;

    std::vector<uint8_t> blob = std::vector<uint8_t>();
    std::vector<std::string> child_nodes = std::vector<std::string>();

    bool priority_val_flag = false;
    bool priority_json_flag = false;
    bool shallow_flag = false;
    int read_tmo = -1;
    std::string write_limit = "";

    std::string backup_node_path = "";
    std::string backup_dir = "";
    std::string backup_filename = "";
    size_t backup_file_size = 0;

    struct fb_esp_stream_info_t stream;

#if defined(ESP32)
    TaskHandle_t stream_task_handle = NULL;
    TaskHandle_t queue_task_handle = NULL;
    size_t stream_task_stack_size = STREAM_TASK_STACK_SIZE;
    size_t queue_task_stack_size = QUEUE_TASK_STACK_SIZE;
    bool stream_task_enable = false;
#endif
};

struct fb_esp_fcs_meta_info_t
{
    std::string name = "";
    std::string bucket = "";
    unsigned long generation = 0;
    std::string contentType = "";
    size_t size = 0;
    std::string etag = "";
    std::string crc32 = "";
    std::string downloadTokens = "";
};

struct fb_esp_fcm_legacy_notification_payload_t
{
    const char *title = "";              //string all
    const char *body = "";               //string all
    const char *icon = "";               //string Andoid, web
    const char *click_action = "";       //string all
    const char *sound = "";              //string iOS, Android
    const char *badge = "";              //number iOS
    const char *subtitle = "";           //string iOS
    const char *body_loc_key = "";       //string iOS, Android
    const char *body_loc_args = "";      //array of string [] iOS, Android
    const char *title_loc_key = "";      //string iOS, Android
    const char *title_loc_args = "";     //array of string [] iOS, Android
    const char *android_channel_id = ""; //string Android
    const char *tag = "";                //string Android
    const char *color = "";              //string Android
};

struct fb_esp_fcm_legacy_http_message_option_t
{
    const char *priority = "";                //string
    const char *collapse_key = "";            //string
    const char *time_to_live = "";            //number
    const char *restricted_package_name = ""; //string
    const char *mutable_content = "";         //boolean
    const char *content_available = "";       //boolean
    const char *dry_run = "";                 //boolean
    const char *direct_boot_ok = "";          //boolean
};

struct fb_esp_fcm_legacy_http_message_payload_t
{
    struct fb_esp_fcm_legacy_notification_payload_t notification;
    const char *data = "";
};

struct fb_esp_fcm_legacy_http_message_target_t
{
    const char *to = "";               //string
    const char *registration_ids = ""; //array of string []
    const char *condition = "";        //string
};

struct fb_esp_fcm_legacy_http_message_info_t
{
    struct fb_esp_fcm_legacy_http_message_target_t targets;
    struct fb_esp_fcm_legacy_http_message_option_t options;
    struct fb_esp_fcm_legacy_http_message_payload_t payloads;
};

struct fb_esp_fcm_http_v1_notification_t
{
    const char *title = ""; //string
    const char *body = "";  //tring
    const char *image = ""; //string
};

struct fb_esp_fcm_http_v1_fcm_options_t
{
    const char *analytics_label = ""; //string, Label associated with the message's analytics data.
};

struct fb_esp_fcm_http_v1_android_fcm_options_t
{
    const char *analytics_label = ""; //string, Label associated with the message's analytics data.
};

struct fb_esp_fcm_http_v1_android_light_settings_color_t
{
    const char *red = "";   //string
    const char *green = ""; //tring
    const char *blue = "";  //tring
    const char *alpha = ""; //tring
};

struct fb_esp_fcm_http_v1_android_light_settings_t
{
    struct fb_esp_fcm_http_v1_android_light_settings_color_t color; //object {}
    const char *light_on_duration = "";                             //string
    const char *light_off_duration = "";                            //tring
};

struct fb_esp_fcm_http_v1_android_noti_t
{
    const char *title = "";                   //string
    const char *body = "";                    //string
    const char *icon = "";                    //string
    const char *color = "";                   //string
    const char *sound = "";                   //string
    const char *tag = "";                     //string
    const char *click_action = "";            //string
    const char *body_loc_key = "";            //string
    const char *body_loc_args = "";           //array of string []
    const char *title_loc_key = "";           //string
    const char *title_loc_args = "";          //array of string []
    const char *channel_id = "";              //string
    const char *ticker = "";                  //string
    const char *sticky = "";                  //boolean
    const char *event_time = "";              //string
    const char *local_only = "";              //boolean
    const char *notification_priority = "";   //enum
    const char *default_sound = "";           //boolean
    const char *default_vibrate_timings = ""; //boolean
    const char *default_light_settings = "";  //boolean
    const char *vibrate_timings = "";         //array of string []
    const char *visibility = "";              //enum
    const char *notification_count = "";      //integer
    struct fb_esp_fcm_http_v1_android_light_settings_t light_settings;
    const char *image = ""; //string
};

struct fb_esp_fcm_http_v1_android_config_t
{
    const char *collapse_key = "";            //string
    const char *priority = "";                //enum
    const char *ttl = "";                     //string
    const char *restricted_package_name = ""; //string
    const char *data = "";                    ///object {} Arbitrary key/value payload
    fb_esp_fcm_http_v1_android_noti_t notification;
    struct fb_esp_fcm_http_v1_android_fcm_options_t fcm_options;
    const char *direct_boot_ok = ""; //boolean
};

struct fb_esp_fcm_http_v1_apns_fcm_opt_t
{
    const char *analytics_label = ""; //string Label associated with the message's analytics data
    const char *image = "";           //tring contains the URL of an image that is going to be displayed in a notification.
};

struct fb_esp_fcm_http_v1_webpush_fcm_opt_t
{
    const char *link = "";            //tring, The link to open when the user clicks on the notification.
    const char *analytics_label = ""; //string, Label associated with the message's analytics data.
};

struct fb_esp_fcm_http_v1_apns_config_t
{
    const char *headers = "";      //object {} http header key/value defined in Apple Push Notification Service
    const char *payload = "";      //object {} APNs payload as a JSON object
    const char *notification = ""; //object {}
    struct fb_esp_fcm_http_v1_apns_fcm_opt_t fcm_options;
};

struct fb_esp_fcm_http_v1_webpush_config_t
{
    const char *headers = "";      //object {} http header key/value defined in webpush protocol.
    const char *data = "";         //object {} abitrary key/value payload
    const char *notification = ""; //object {} Web Notification options as a JSON object
    struct fb_esp_fcm_http_v1_webpush_fcm_opt_t fcm_options;
};

struct fb_esp_fcm_http_v1_message_info_t
{
    const char *token = "";     //tring
    const char *topic = "";     //tring
    const char *condition = ""; //tring
    struct fb_esp_fcm_http_v1_fcm_options_t fcm_options;
    struct fb_esp_fcm_http_v1_notification_t notification;
    const char *data = ""; //object {} abitrary key/value payload
    struct fb_esp_fcm_http_v1_android_config_t android;
    struct fb_esp_fcm_http_v1_apns_config_t apns;
    struct fb_esp_fcm_http_v1_webpush_config_t webpush;
};

struct fb_esp_fcm_info_t
{
    std::string payload = "";
};

struct fb_esp_gcs_info_t
{
    std::string local_filename = "";
    std::string remote_filename = "";
    std::string bucketID = "";
    std::string mime = "";
    std::string location = "";
    fb_esp_gcs_request_type requestType = fb_esp_gcs_request_type_undefined;
    size_t fileSize = 0;
    fb_esp_mem_storage_type storage_type = mem_storage_type_undefined;
    int contentLength = 0;
};

struct fb_esp_fcs_info_t
{
    fb_esp_fcs_request_type requestType = fb_esp_fcs_request_type_undefined;
    size_t fileSize = 0;
    fb_esp_mem_storage_type storage_type = mem_storage_type_undefined;
    int contentLength = 0;
    struct fb_esp_fcs_meta_info_t meta;
    struct fb_esp_fcs_file_list_t files;
};

struct fb_esp_session_info_t
{
    FirebaseJson json;
    FirebaseJsonArray arr;
    FirebaseJsonData data;
    fb_esp_con_mode con_mode = fb_esp_con_mode_undefined;
    bool buffer_ovf = false;
    bool chunked_encoding = false;
    bool connected = false;
    bool classic_request = false;

    uint16_t resp_size = 1024;
    int http_code = -1000;
    int content_length = 0;
    std::string error = "";
    struct fb_esp_rtdb_info_t rtdb;
    struct fb_esp_gcs_info_t gcs;
    struct fb_esp_fcs_info_t fcs;
    struct fb_esp_fcm_info_t fcm;

#if defined(ESP8266)
    uint16_t bssl_rx_size = 512;
    uint16_t bssl_tx_size = 512;
#endif
};

struct fb_esp_gcs_req_t
{
    std::string sourceFileName = "";
    fb_esp_mem_storage_type storageType = mem_storage_type_undefined;
    fb_esp_gcs_request_type requestType = fb_esp_gcs_request_type_undefined;
    std::string targetFileName = "";
    std::string mime = "";
};

struct fb_esp_fcs_req_t
{
    std::string remoteFileName = "";
    std::string localFileName = "";
    std::string bucketID = "";
    std::string mime = "";
    fb_esp_mem_storage_type storageType = mem_storage_type_undefined;
    fb_esp_fcs_request_type requestType = fb_esp_fcs_request_type_undefined;
};

typedef struct fb_esp_fcs_meta_info_t FileMetaInfo;
typedef struct fb_esp_fcs_file_list_t FileList;
typedef struct fb_esp_fcs_file_list_item_t FileItem;
typedef struct fb_esp_auth_signin_provider_t FirebaseAuth;
typedef struct fb_esp_cfg_t FirebaseConfig;
typedef struct fb_esp_fcm_legacy_http_message_info_t FCM_Legacy_HTTP_Message;
typedef struct fb_esp_fcm_http_v1_message_info_t FCM_HTTPv1_JSON_Message;

typedef std::function<void(void)> callback_function_t;


static const char fb_esp_pgm_str_1[] PROGMEM = "/";
static const char fb_esp_pgm_str_2[] PROGMEM = ".json?auth=";
static const char fb_esp_pgm_str_3[] PROGMEM = "\"";
static const char fb_esp_pgm_str_4[] PROGMEM = ".";
static const char fb_esp_pgm_str_5[] PROGMEM = "HTTP/1.1 ";
static const char fb_esp_pgm_str_6[] PROGMEM = " ";
static const char fb_esp_pgm_str_7[] PROGMEM = ":";
static const char fb_esp_pgm_str_8[] PROGMEM = "Content-Type: ";
static const char fb_esp_pgm_str_9[] PROGMEM = "text/event-stream";
static const char fb_esp_pgm_str_10[] PROGMEM = "Connection: ";
static const char fb_esp_pgm_str_11[] PROGMEM = "keep-alive";
static const char fb_esp_pgm_str_12[] PROGMEM = "Content-Length: ";
static const char fb_esp_pgm_str_13[] PROGMEM = "event: ";
static const char fb_esp_pgm_str_14[] PROGMEM = "data: ";
static const char fb_esp_pgm_str_15[] PROGMEM = "put";
static const char fb_esp_pgm_str_16[] PROGMEM = "patch";
static const char fb_esp_pgm_str_17[] PROGMEM = "\"path\":\"";
static const char fb_esp_pgm_str_18[] PROGMEM = "\"data\":";
static const char fb_esp_pgm_str_19[] PROGMEM = "null";
static const char fb_esp_pgm_str_20[] PROGMEM = "{\"name\":\"";
static const char fb_esp_pgm_str_21[] PROGMEM = "\r\n";
static const char fb_esp_pgm_str_22[] PROGMEM = "GET ";
static const char fb_esp_pgm_str_23[] PROGMEM = "PUT";
static const char fb_esp_pgm_str_24[] PROGMEM = "POST";
static const char fb_esp_pgm_str_25[] PROGMEM = "GET";
static const char fb_esp_pgm_str_26[] PROGMEM = "PATCH";
static const char fb_esp_pgm_str_27[] PROGMEM = "DELETE";
static const char fb_esp_pgm_str_28[] PROGMEM = "&download=";
static const char fb_esp_pgm_str_29[] PROGMEM = "&print=silent";
static const char fb_esp_pgm_str_30[] PROGMEM = " HTTP/1.1\r\n";
static const char fb_esp_pgm_str_31[] PROGMEM = "Host: ";
static const char fb_esp_pgm_str_32[] PROGMEM = "User-Agent: ESP\r\n";
static const char fb_esp_pgm_str_33[] PROGMEM = "X-Firebase-Decoding: 1\r\n";
static const char fb_esp_pgm_str_34[] PROGMEM = "Connection: close\r\n";
static const char fb_esp_pgm_str_35[] PROGMEM = "Accept: text/event-stream\r\n";
static const char fb_esp_pgm_str_36[] PROGMEM = "Connection: keep-alive\r\n";
static const char fb_esp_pgm_str_37[] PROGMEM = "Keep-Alive: timeout=30, max=100\r\n";
static const char fb_esp_pgm_str_38[] PROGMEM = "Accept-Encoding: identity;q=1,chunked;q=0.1,*;q=0\r\n";
static const char fb_esp_pgm_str_39[] PROGMEM = "connection refused";
static const char fb_esp_pgm_str_40[] PROGMEM = "send header failed";
static const char fb_esp_pgm_str_41[] PROGMEM = "send payload failed";
static const char fb_esp_pgm_str_42[] PROGMEM = "not connected";
static const char fb_esp_pgm_str_43[] PROGMEM = "connection lost";
static const char fb_esp_pgm_str_44[] PROGMEM = "no HTTP server";
static const char fb_esp_pgm_str_45[] PROGMEM = "bad request";
static const char fb_esp_pgm_str_46[] PROGMEM = "non-authoriative information";
static const char fb_esp_pgm_str_47[] PROGMEM = "no content";
static const char fb_esp_pgm_str_48[] PROGMEM = "moved permanently";
static const char fb_esp_pgm_str_49[] PROGMEM = "use proxy";
static const char fb_esp_pgm_str_50[] PROGMEM = "temporary redirect";
static const char fb_esp_pgm_str_51[] PROGMEM = "permanent redirect";
static const char fb_esp_pgm_str_52[] PROGMEM = "unauthorized";
static const char fb_esp_pgm_str_53[] PROGMEM = "forbidden";
static const char fb_esp_pgm_str_54[] PROGMEM = "not found";
static const char fb_esp_pgm_str_55[] PROGMEM = "method not allow";
static const char fb_esp_pgm_str_56[] PROGMEM = "not acceptable";
static const char fb_esp_pgm_str_57[] PROGMEM = "proxy authentication required";
static const char fb_esp_pgm_str_58[] PROGMEM = "request timeout";
static const char fb_esp_pgm_str_59[] PROGMEM = "length required";
static const char fb_esp_pgm_str_60[] PROGMEM = "too many requests";
static const char fb_esp_pgm_str_61[] PROGMEM = "request header fields too larg";
static const char fb_esp_pgm_str_62[] PROGMEM = "internal server error";
static const char fb_esp_pgm_str_63[] PROGMEM = "bad gateway";
static const char fb_esp_pgm_str_64[] PROGMEM = "service unavailable";
static const char fb_esp_pgm_str_65[] PROGMEM = "gateway timeout";
static const char fb_esp_pgm_str_66[] PROGMEM = "http version not support";
static const char fb_esp_pgm_str_67[] PROGMEM = "network authentication required";
static const char fb_esp_pgm_str_68[] PROGMEM = "data buffer overflow";
static const char fb_esp_pgm_str_69[] PROGMEM = "read Timeout";
static const char fb_esp_pgm_str_70[] PROGMEM = "data type mismatch";
static const char fb_esp_pgm_str_71[] PROGMEM = "path not exist";
static const char fb_esp_pgm_str_72[] PROGMEM = "task";
static const char fb_esp_pgm_str_73[] PROGMEM = "/esp.x";
static const char fb_esp_pgm_str_74[] PROGMEM = "json";
static const char fb_esp_pgm_str_75[] PROGMEM = "string";
static const char fb_esp_pgm_str_76[] PROGMEM = "float";
static const char fb_esp_pgm_str_77[] PROGMEM = "int";
static const char fb_esp_pgm_str_78[] PROGMEM = "null";
static const char fb_esp_pgm_str_79[] PROGMEM = ";";
static const char fb_esp_pgm_str_80[] PROGMEM = "Content-Disposition: ";
static const char fb_esp_pgm_str_81[] PROGMEM = "application/octet-stream";
static const char fb_esp_pgm_str_82[] PROGMEM = "attachment";
static const char fb_esp_pgm_str_83[] PROGMEM = "The backup file is not exist";
static const char fb_esp_pgm_str_84[] PROGMEM = "The SD card is in use";
static const char fb_esp_pgm_str_85[] PROGMEM = "The SD card is not available";
static const char fb_esp_pgm_str_86[] PROGMEM = "Could not read/write the backup file";
static const char fb_esp_pgm_str_87[] PROGMEM = "Transmission error, ";
static const char fb_esp_pgm_str_88[] PROGMEM = "Node path is not exist";
static const char fb_esp_pgm_str_89[] PROGMEM = ".json";
static const char fb_esp_pgm_str_90[] PROGMEM = "/root.json";
static const char fb_esp_pgm_str_91[] PROGMEM = "blob";
static const char fb_esp_pgm_str_92[] PROGMEM = "\"blob,base64,";
static const char fb_esp_pgm_str_93[] PROGMEM = "\"file,base64,";
static const char fb_esp_pgm_str_94[] PROGMEM = "http connection was used by other processes";
static const char fb_esp_pgm_str_95[] PROGMEM = "Location: ";
static const char fb_esp_pgm_str_96[] PROGMEM = "&orderBy=";
static const char fb_esp_pgm_str_97[] PROGMEM = "&limitToFirst=";
static const char fb_esp_pgm_str_98[] PROGMEM = "&limitToLast=";
static const char fb_esp_pgm_str_99[] PROGMEM = "&startAt=";
static const char fb_esp_pgm_str_100[] PROGMEM = "&endAt=";
static const char fb_esp_pgm_str_101[] PROGMEM = "&equalTo=";
static const char fb_esp_pgm_str_102[] PROGMEM = "\"error\" : ";
static const char fb_esp_pgm_str_103[] PROGMEM = "/.settings/rules";
static const char fb_esp_pgm_str_104[] PROGMEM = "{\"status\":\"ok\"}";
static const char fb_esp_pgm_str_105[] PROGMEM = "boolean";
static const char fb_esp_pgm_str_106[] PROGMEM = "false";
static const char fb_esp_pgm_str_107[] PROGMEM = "true";
static const char fb_esp_pgm_str_108[] PROGMEM = "double";
static const char fb_esp_pgm_str_109[] PROGMEM = "cancel";
static const char fb_esp_pgm_str_110[] PROGMEM = "auth_revoked";
static const char fb_esp_pgm_str_111[] PROGMEM = "http://";
static const char fb_esp_pgm_str_112[] PROGMEM = "https://";
static const char fb_esp_pgm_str_113[] PROGMEM = "_stream";
static const char fb_esp_pgm_str_114[] PROGMEM = "_error_queue";
static const char fb_esp_pgm_str_115[] PROGMEM = "get";
static const char fb_esp_pgm_str_116[] PROGMEM = "set";
static const char fb_esp_pgm_str_117[] PROGMEM = "push";
static const char fb_esp_pgm_str_118[] PROGMEM = "update";
static const char fb_esp_pgm_str_119[] PROGMEM = "delete";
static const char fb_esp_pgm_str_120[] PROGMEM = "googleapis.com";
static const char fb_esp_pgm_str_121[] PROGMEM = "/fcm/send";
static const char fb_esp_pgm_str_122[] PROGMEM = "notification";
static const char fb_esp_pgm_str_123[] PROGMEM = "body";
static const char fb_esp_pgm_str_124[] PROGMEM = "icon";
static const char fb_esp_pgm_str_125[] PROGMEM = "click_action";
static const char fb_esp_pgm_str_126[] PROGMEM = "sound";
static const char fb_esp_pgm_str_127[] PROGMEM = "}";
static const char fb_esp_pgm_str_128[] PROGMEM = "to";
static const char fb_esp_pgm_str_129[] PROGMEM = "application/json";
static const char fb_esp_pgm_str_130[] PROGMEM = "registration_ids";
static const char fb_esp_pgm_str_131[] PROGMEM = "Authorization: key=";
static const char fb_esp_pgm_str_132[] PROGMEM = ",";
static const char fb_esp_pgm_str_133[] PROGMEM = "]";
static const char fb_esp_pgm_str_134[] PROGMEM = "/topics/";
static const char fb_esp_pgm_str_135[] PROGMEM = "data";
static const char fb_esp_pgm_str_136[] PROGMEM = "priority";
static const char fb_esp_pgm_str_137[] PROGMEM = "time_to_live";
static const char fb_esp_pgm_str_138[] PROGMEM = "collapse_key";
static const char fb_esp_pgm_str_139[] PROGMEM = "\"multicast_id\":";
static const char fb_esp_pgm_str_140[] PROGMEM = "\"success\":";
static const char fb_esp_pgm_str_141[] PROGMEM = "\"failure\":";
static const char fb_esp_pgm_str_142[] PROGMEM = "\"canonical_ids\":";
static const char fb_esp_pgm_str_143[] PROGMEM = "\"results\":";
static const char fb_esp_pgm_str_144[] PROGMEM = "registration_id";
static const char fb_esp_pgm_str_145[] PROGMEM = "No device registration ID provided";
static const char fb_esp_pgm_str_146[] PROGMEM = "No server key provided";
static const char fb_esp_pgm_str_147[] PROGMEM = "restricted_package_name";
static const char fb_esp_pgm_str_148[] PROGMEM = "X-Firebase-ETag: true\r\n";
static const char fb_esp_pgm_str_149[] PROGMEM = "if-match: ";
static const char fb_esp_pgm_str_150[] PROGMEM = "ETag: ";
static const char fb_esp_pgm_str_151[] PROGMEM = "null_etag";
static const char fb_esp_pgm_str_152[] PROGMEM = "Precondition Failed (ETag does not match)";
static const char fb_esp_pgm_str_153[] PROGMEM = "X-HTTP-Method-Override: ";
static const char fb_esp_pgm_str_154[] PROGMEM = "{\".sv\": \"timestamp\"}";
static const char fb_esp_pgm_str_155[] PROGMEM = "&shallow=true";
static const char fb_esp_pgm_str_156[] PROGMEM = "/.priority";
static const char fb_esp_pgm_str_157[] PROGMEM = ",\".priority\":";
static const char fb_esp_pgm_str_158[] PROGMEM = "&timeout=";
static const char fb_esp_pgm_str_159[] PROGMEM = "ms";
static const char fb_esp_pgm_str_160[] PROGMEM = "&writeSizeLimit=";
static const char fb_esp_pgm_str_161[] PROGMEM = "{\".value\":";
static const char fb_esp_pgm_str_162[] PROGMEM = "&format=export";
static const char fb_esp_pgm_str_163[] PROGMEM = "{";
static const char fb_esp_pgm_str_164[] PROGMEM = "Flash memory was not ready";
static const char fb_esp_pgm_str_165[] PROGMEM = "array";
static const char fb_esp_pgm_str_166[] PROGMEM = "\".sv\"";
static const char fb_esp_pgm_str_167[] PROGMEM = "Transfer-Encoding: ";
static const char fb_esp_pgm_str_168[] PROGMEM = "chunked";
static const char fb_esp_pgm_str_169[] PROGMEM = "Maximum Redirection reached";
static const char fb_esp_pgm_str_170[] PROGMEM = "?auth=";
static const char fb_esp_pgm_str_171[] PROGMEM = "&auth=";
static const char fb_esp_pgm_str_172[] PROGMEM = "&";
static const char fb_esp_pgm_str_173[] PROGMEM = "?";
static const char fb_esp_pgm_str_174[] PROGMEM = "#";
static const char fb_esp_pgm_str_175[] PROGMEM = "localId";
static const char fb_esp_pgm_str_176[] PROGMEM = "error";
static const char fb_esp_pgm_str_177[] PROGMEM = "token exchange failed";
static const char fb_esp_pgm_str_178[] PROGMEM = "JWT token signing failed";
static const char fb_esp_pgm_str_179[] PROGMEM = "RSA private key parsing failed";
static const char fb_esp_pgm_str_180[] PROGMEM = "\n";
static const char fb_esp_pgm_str_181[] PROGMEM = "\r\n\r\n";
static const char fb_esp_pgm_str_182[] PROGMEM = "[";
static const char fb_esp_pgm_str_183[] PROGMEM = "file";
static const char fb_esp_pgm_str_184[] PROGMEM = "/fb_bin_0.tmp";
static const char fb_esp_pgm_str_185[] PROGMEM = "The backup data should be the JSON object";
static const char fb_esp_pgm_str_186[] PROGMEM = "object";
static const char fb_esp_pgm_str_187[] PROGMEM = "pool.ntp.org";
static const char fb_esp_pgm_str_188[] PROGMEM = "time.nist.gov";
static const char fb_esp_pgm_str_189[] PROGMEM = "payload too large";
static const char fb_esp_pgm_str_190[] PROGMEM = "cannot config time";
static const char fb_esp_pgm_str_191[] PROGMEM = "incomplete SSL client data";
static const char fb_esp_pgm_str_192[] PROGMEM = "File I/O error";
static const char fb_esp_pgm_str_193[] PROGMEM = "www";
static const char fb_esp_pgm_str_194[] PROGMEM = "/identitytoolkit/v3/relyingparty/";
static const char fb_esp_pgm_str_195[] PROGMEM = "verifyPassword?key=";
static const char fb_esp_pgm_str_196[] PROGMEM = "email";
static const char fb_esp_pgm_str_197[] PROGMEM = "password";
static const char fb_esp_pgm_str_198[] PROGMEM = "returnSecureToken";
static const char fb_esp_pgm_str_199[] PROGMEM = "registered";
static const char fb_esp_pgm_str_200[] PROGMEM = "idToken";
static const char fb_esp_pgm_str_201[] PROGMEM = "refreshToken";
static const char fb_esp_pgm_str_202[] PROGMEM = "expiresIn";
static const char fb_esp_pgm_str_203[] PROGMEM = "securetoken";
static const char fb_esp_pgm_str_204[] PROGMEM = "/v1/token?key=";
static const char fb_esp_pgm_str_205[] PROGMEM = "grantType";
static const char fb_esp_pgm_str_206[] PROGMEM = "refresh_token";
static const char fb_esp_pgm_str_207[] PROGMEM = "refreshToken";
static const char fb_esp_pgm_str_208[] PROGMEM = "id_token";
static const char fb_esp_pgm_str_209[] PROGMEM = "refresh_token";
static const char fb_esp_pgm_str_210[] PROGMEM = "expires_in";
static const char fb_esp_pgm_str_211[] PROGMEM = "waiting for token to be ready";
static const char fb_esp_pgm_str_212[] PROGMEM = "iss";
static const char fb_esp_pgm_str_213[] PROGMEM = "sub";
static const char fb_esp_pgm_str_214[] PROGMEM = "aud";
static const char fb_esp_pgm_str_215[] PROGMEM = "exp";
static const char fb_esp_pgm_str_216[] PROGMEM = "/token";
static const char fb_esp_pgm_str_217[] PROGMEM = "/oauth2/v4/token";
static const char fb_esp_pgm_str_218[] PROGMEM = "iat";
static const char fb_esp_pgm_str_219[] PROGMEM = "auth";
static const char fb_esp_pgm_str_220[] PROGMEM = "scope";
static const char fb_esp_pgm_str_221[] PROGMEM = "devstorage.full_control";
static const char fb_esp_pgm_str_222[] PROGMEM = "datastore";
static const char fb_esp_pgm_str_223[] PROGMEM = "userinfo.email";
static const char fb_esp_pgm_str_224[] PROGMEM = "firebase.database";
static const char fb_esp_pgm_str_225[] PROGMEM = "cloud-platform";
static const char fb_esp_pgm_str_226[] PROGMEM = "firestore";
static const char fb_esp_pgm_str_227[] PROGMEM = "grant_type";
static const char fb_esp_pgm_str_228[] PROGMEM = "urn:ietf:params:oauth:grant-type:jwt-bearer";
static const char fb_esp_pgm_str_229[] PROGMEM = "assertion";
static const char fb_esp_pgm_str_230[] PROGMEM = "could not get time from NTP server";
static const char fb_esp_pgm_str_231[] PROGMEM = "/google.identity.identitytoolkit.v1.IdentityToolkit";
static const char fb_esp_pgm_str_232[] PROGMEM = "verifyCustomToken?key=";
static const char fb_esp_pgm_str_233[] PROGMEM = "token";
static const char fb_esp_pgm_str_234[] PROGMEM = "JWT";
static const char fb_esp_pgm_str_235[] PROGMEM = "access_token";
static const char fb_esp_pgm_str_236[] PROGMEM = "token_type";
static const char fb_esp_pgm_str_237[] PROGMEM = "Authorization: ";
static const char fb_esp_pgm_str_238[] PROGMEM = ".json";
static const char fb_esp_pgm_str_239[] PROGMEM = "alg";
static const char fb_esp_pgm_str_240[] PROGMEM = "typ";
static const char fb_esp_pgm_str_241[] PROGMEM = "kid";
static const char fb_esp_pgm_str_242[] PROGMEM = "RS256";
static const char fb_esp_pgm_str_243[] PROGMEM = "type";
static const char fb_esp_pgm_str_244[] PROGMEM = "service_account";
static const char fb_esp_pgm_str_245[] PROGMEM = "project_id";
static const char fb_esp_pgm_str_246[] PROGMEM = "private_key_id";
static const char fb_esp_pgm_str_247[] PROGMEM = "private_key";
static const char fb_esp_pgm_str_248[] PROGMEM = "client_email";
static const char fb_esp_pgm_str_249[] PROGMEM = "fcm";
static const char fb_esp_pgm_str_250[] PROGMEM = "identitytoolkit";
static const char fb_esp_pgm_str_251[] PROGMEM = "oauth2";
static const char fb_esp_pgm_str_252[] PROGMEM = "token is not ready";
static const char fb_esp_pgm_str_253[] PROGMEM = "client_id";
static const char fb_esp_pgm_str_254[] PROGMEM = "uid";
static const char fb_esp_pgm_str_255[] PROGMEM = "claims";
static const char fb_esp_pgm_str_256[] PROGMEM = "Firebase authentication was not initialized";
static const char fb_esp_pgm_str_257[] PROGMEM = "error/code";
static const char fb_esp_pgm_str_258[] PROGMEM = "error/message";
static const char fb_esp_pgm_str_259[] PROGMEM = "/v1/accounts:signUp?key=";
static const char fb_esp_pgm_str_260[] PROGMEM = "requestType";
static const char fb_esp_pgm_str_261[] PROGMEM = "VERIFY_EMAIL";
static const char fb_esp_pgm_str_262[] PROGMEM = "getOobConfirmationCode?key=";
static const char fb_esp_pgm_str_263[] PROGMEM = "PASSWORD_RESET";
static const char fb_esp_pgm_str_264[] PROGMEM = "could not open file";
static const char fb_esp_pgm_str_265[] PROGMEM = "firebasestorage.";
static const char fb_esp_pgm_str_266[] PROGMEM = "/v0/b/";
static const char fb_esp_pgm_str_267[] PROGMEM = "/o";
static const char fb_esp_pgm_str_268[] PROGMEM = "name=";
static const char fb_esp_pgm_str_269[] PROGMEM = "alt=media";
static const char fb_esp_pgm_str_270[] PROGMEM = "Firebase ";
static const char fb_esp_pgm_str_271[] PROGMEM = "Bearer ";
static const char fb_esp_pgm_str_272[] PROGMEM = "downloadTokens";
static const char fb_esp_pgm_str_273[] PROGMEM = "token=";
static const char fb_esp_pgm_str_274[] PROGMEM = "name";
static const char fb_esp_pgm_str_275[] PROGMEM = "bucket";
static const char fb_esp_pgm_str_276[] PROGMEM = "generation";
static const char fb_esp_pgm_str_277[] PROGMEM = "contentType";
static const char fb_esp_pgm_str_278[] PROGMEM = "size";
static const char fb_esp_pgm_str_279[] PROGMEM = "etag";
static const char fb_esp_pgm_str_280[] PROGMEM = "crc32";
static const char fb_esp_pgm_str_281[] PROGMEM = "dry_run";
static const char fb_esp_pgm_str_282[] PROGMEM = "condition";
static const char fb_esp_pgm_str_283[] PROGMEM = "content_available";
static const char fb_esp_pgm_str_284[] PROGMEM = "mutable_content";

static const char fb_esp_pgm_str_285[] PROGMEM = "title";
static const char fb_esp_pgm_str_286[] PROGMEM = "badge";
static const char fb_esp_pgm_str_287[] PROGMEM = "subtitle";
static const char fb_esp_pgm_str_288[] PROGMEM = "body_loc_key";
static const char fb_esp_pgm_str_289[] PROGMEM = "body_loc_args";
static const char fb_esp_pgm_str_290[] PROGMEM = "title_loc_key";
static const char fb_esp_pgm_str_291[] PROGMEM = "title_loc_args";
static const char fb_esp_pgm_str_292[] PROGMEM = "android_channel_id";
static const char fb_esp_pgm_str_293[] PROGMEM = "tag";
static const char fb_esp_pgm_str_294[] PROGMEM = "color";
static const char fb_esp_pgm_str_295[] PROGMEM = "message";
static const char fb_esp_pgm_str_296[] PROGMEM = "topic";
static const char fb_esp_pgm_str_297[] PROGMEM = "image";
static const char fb_esp_pgm_str_298[] PROGMEM = "fcm_options";
static const char fb_esp_pgm_str_299[] PROGMEM = "analytics_label";
static const char fb_esp_pgm_str_300[] PROGMEM = "android";
static const char fb_esp_pgm_str_301[] PROGMEM = "webpush";
static const char fb_esp_pgm_str_302[] PROGMEM = "apns";
static const char fb_esp_pgm_str_303[] PROGMEM = "ttl";
static const char fb_esp_pgm_str_304[] PROGMEM = "channel_id";
static const char fb_esp_pgm_str_305[] PROGMEM = "ticker";
static const char fb_esp_pgm_str_306[] PROGMEM = "sticky";
static const char fb_esp_pgm_str_307[] PROGMEM = "event_time";
static const char fb_esp_pgm_str_308[] PROGMEM = "local_only";
static const char fb_esp_pgm_str_309[] PROGMEM = "notification_priority";
static const char fb_esp_pgm_str_310[] PROGMEM = "default_sound";
static const char fb_esp_pgm_str_311[] PROGMEM = "default_vibrate_timings";
static const char fb_esp_pgm_str_312[] PROGMEM = "default_light_settings";
static const char fb_esp_pgm_str_313[] PROGMEM = "vibrate_timings";
static const char fb_esp_pgm_str_314[] PROGMEM = "visibility";
static const char fb_esp_pgm_str_315[] PROGMEM = "notification_count";
static const char fb_esp_pgm_str_316[] PROGMEM = "light_settings";
static const char fb_esp_pgm_str_317[] PROGMEM = "red";
static const char fb_esp_pgm_str_318[] PROGMEM = "green";
static const char fb_esp_pgm_str_319[] PROGMEM = "blue";
static const char fb_esp_pgm_str_320[] PROGMEM = "alpha";
static const char fb_esp_pgm_str_321[] PROGMEM = "light_on_duration";
static const char fb_esp_pgm_str_322[] PROGMEM = "light_off_duration";
static const char fb_esp_pgm_str_323[] PROGMEM = "direct_boot_ok";
static const char fb_esp_pgm_str_324[] PROGMEM = "headers";
static const char fb_esp_pgm_str_325[] PROGMEM = "link";
static const char fb_esp_pgm_str_326[] PROGMEM = "/v1/projects/";
static const char fb_esp_pgm_str_327[] PROGMEM = "/messages:send";
static const char fb_esp_pgm_str_328[] PROGMEM = "OAuth2.0 authentication required";
static const char fb_esp_pgm_str_329[] PROGMEM = "iid";
static const char fb_esp_pgm_str_330[] PROGMEM = "/iid/v1";
static const char fb_esp_pgm_str_331[] PROGMEM = ":batchAdd";
static const char fb_esp_pgm_str_332[] PROGMEM = ":batchRemove";
static const char fb_esp_pgm_str_333[] PROGMEM = ":batchImport";
static const char fb_esp_pgm_str_334[] PROGMEM = "registration_tokens";
static const char fb_esp_pgm_str_335[] PROGMEM = "/iid/info/";
static const char fb_esp_pgm_str_336[] PROGMEM = "?details=true";
static const char fb_esp_pgm_str_337[] PROGMEM = "application";
static const char fb_esp_pgm_str_338[] PROGMEM = "sandbox";
static const char fb_esp_pgm_str_339[] PROGMEM = "apns_tokens";

static const unsigned char fb_esp_base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char fb_esp_boundary_table[] PROGMEM = "=_abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

#endif