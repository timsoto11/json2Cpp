#include <string>
#include <vector>

struct Enforcement
{
    int main_lane_signal;
    int ac_sync_delay;
    int violation_window_in_16th_sec;
    int violation_per_phase;
    int min_quality;
    int violation_per_reset_lane;
    int flash1_binary;
    int flash2_binary;
    int flash3_binary;
    int unit;
    int operation_mode;
    int vehicle_detector;
    int light_phase_enforcement;
    int car_speed_threshold;
    int truck_speed_threshold;
    int speed_flash_spread_1st;
    int site_max_plausible_speed;
    int site_implausible_speed_action;
    int speed_time_2nd_shot;
    int speed_dist_2nd_shot;
    int speed_flash_spread_2nd;
    int atr_lane_number;
    int spare1;
    int vehicle_length;
    int mfmp_configuration;
    int noise_filter;
    int dark_phase_delay;
    int spare6;
    int spare7;
    int spare8;
    int spare9;
    int spare10;
    int roadway_heading_tolerance;
    int system_latency_compensation;
    int length_override;
    int spare14;
    int spare15;
    int crosswalk_dwell_time;
    int spare17;
    int spare18;
    int max_ashot_speed;
    int bshot_speed_modifier;
    int max_num_violations_per_phase;
    int suspend_red_phase;
    int rl_high_speed_threshold;
    int agile_radar_type;
    int bshot_holdoff;
    int min_vehicle_length;
    int max_vehicle_length;
    int length_sensitivity;
    int transverse_velocity;
    int rail_cross_grace_time;
    int yellow_phase_calibration;
    int spare32;
    int speedboard;
    int speedboard_baud;
    int speedboard_display;
    int speedboard_lane;
    int spare37;
    int spare38;
    int spare39;
    int spare40;
    int spare41;
    int split_car_filter;
};

struct LaneConfigStruct
{
    int logical_lane_name;
    int special_monitoring_options;
    int lane_signal_channel;
    int signal_configuration;
    int signal_fault_monitoring;
    int rail_cross_signal;
    int min_yellow_in_10th_sec;
    int position_state_table_selector;
    int red_grace_in_100th_sec;
    int speed_adj_factor_in_128th;
    int speed_handling_modifiers;
    int stopbar_detection_modifier;
    int red_speed_threshold;
    int rl_1st_photo_flash_spread;
    int rl_time_to_2nd_photo_in_10th_sec;
    int rl_dist_to_2nd_photo;
    int rl_2nd_photo_flash_spread;
    int lane_far_edge_to_datum;
    int enforcement_direction;
    int stopbar_mid_lane_distance;
};

struct LaneConfig
{
    LaneConfigStruct lane_configStruct;
};

struct CameraSetupStruct
{
    std::string alias;
    std::string lane_coverage;
    int capture_mode;
    int delay_mode;
    int delay_distance;
    int delay_time;
    int photos_per_trigger;
    int interval_mode;
    int interval_distance;
    int interval_time;
};

struct CameraSetup
{
    CameraSetupStruct camera_setupStruct;
};

struct RemoteCameraSetupStruct
{
    std::string alias;
    std::string lane_coverage;
    std::string ip_address;
    int capture_mode;
    int delay_mode;
    int delay_distance;
    int delay_time;
    int photos_per_trigger;
    int interval_mode;
    int interval_distance;
    int interval_time;
};

struct RemoteCameraSetup
{
    RemoteCameraSetupStruct remote_camera_setupStruct;
};

struct FlashStruct
{
    int cam;
    int channel;
    bool violation;
};

struct Flash
{
    FlashStruct flashStruct;
};

struct Video
{
    int clip_seconds_pre_event;
    int clip_seconds_post_event;
};

struct TypeCustomizationStruct
{
    std::string event_type;
    bool show_time_lapse;
    bool show_num_shots;
    int height;
    int font_size;
    bool speed_type;
    int tip_decimal;
    int amber_decimal;
    int time_decimal;
    std::string time_stamp;
    std::string location_description;
    std::string violation_type;
    std::string event_number;
    std::string image_seq;
    std::string lane;
    std::string speed;
    std::string time_into_phase;
    std::string amber_duration;
    std::string officer_id;
    std::string posted_speed;
    std::string radar_serial_number;
};

struct TypeCustomization
{
    TypeCustomizationStruct type_customizationStruct;
};

struct Databar
{
    std::string font;
    std::string language;
    std::vector<TypeCustomization> type_customization;
};

struct IncidentRoisStruct
{
    std::string event_type;
    int lane;
    int cam;
    bool remote;
    std::string type;
    double x0;
    double y0;
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
};

struct IncidentRois
{
    IncidentRoisStruct incident_roisStruct;
};

struct DailySpeedThresholdScheduleStruct
{
    std::string start_time;
    std::string end_time;
    int speed_threshold;
    bool sunday;
    bool monday;
    bool tuesday;
    bool wednesday;
    bool thursday;
    bool friday;
    bool saturday;
};

struct DailySpeedThresholdSchedule
{
    DailySpeedThresholdScheduleStruct daily_speed_threshold_scheduleStruct;
};

struct SpeedEnforcementExclusionsStruct
{
    std::string start_date;
    std::string end_date;
};

struct SpeedEnforcementExclusions
{
    SpeedEnforcementExclusionsStruct speed_enforcement_exclusionsStruct;
};

struct NtorEnforcementScheduleStruct
{
    std::string start_time;
    std::string end_time;
    int lane;
    bool sunday;
    bool monday;
    bool tuesday;
    bool wednesday;
    bool thursday;
    bool friday;
    bool saturday;
};

struct NtorEnforcementSchedule
{
    NtorEnforcementScheduleStruct ntor_enforcement_scheduleStruct;
};

struct RedLightStruct
{
    std::string start_time;
    std::string end_time;
    bool sunday;
    bool monday;
    bool tuesday;
    bool wednesday;
    bool thursday;
    bool friday;
    bool saturday;
};

struct RedLight
{
    RedLightStruct red_lightStruct;
};

struct Scheduler
{
    std::vector<DailySpeedThresholdSchedule> daily_speed_threshold_schedule;
    std::vector<SpeedEnforcementExclusions> speed_enforcement_exclusions;
    std::vector<NtorEnforcementSchedule> ntor_enforcement_schedule;
    std::vector<RedLight> red_light;
};

struct IncidentProcessingStruct
{
    std::string filter_original_type;
    std::string filter_lanes;
    std::string filter_speed_comparator;
    int filter_speed_threshold;
    int filter_timeout;
    std::string filter_event;
    std::string filter_action;
    std::string filter_new_class;
    std::string filter_notes;
};

struct IncidentProcessing
{
    IncidentProcessingStruct incident_processingStruct;
};

struct LocationAliasStruct
{
    std::string alias;
    std::string alias_description;
    int camera_id;
};

struct LocationAlias
{
    LocationAliasStruct location_aliasStruct;
};

struct Miscellaneous
{
    int cool_down;
    int posted_speed_limit;
    bool quiet_mode;
    std::string sensor_serial_number;
    bool encryption;
    std::string setup_operator;
};

struct ViolationValidation
{
    bool enabled;
    bool write_to_db;
};

struct Roi
{
    double x0;
    double y0;
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
};

struct Direction
{
    double x0;
    double y0;
    double x1;
    double y1;
};

struct RegionsStruct
{
    bool enabled;
    int cam;
    int lane;
    bool remote;
    Roi roi;
    Direction direction;
};

struct Regions
{
    RegionsStruct regionsStruct;
};

struct CrosswalkRejection
{
    bool cw_time_filter;
    bool enabled;
    std::vector<Regions> regions;
};

struct Roi
{
    double x0;
    double y0;
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
};

struct RegionsStruct
{
    bool enabled;
    int cam;
    int lane;
    bool remote;
    Roi roi;
};

struct Regions
{
    RegionsStruct regionsStruct;
};

struct IntelligentRoi
{
    bool enabled;
    std::vector<Regions> regions;
};

struct ComputerVision
{
    ViolationValidation violation_validation;
    bool lpr;
    CrosswalkRejection crosswalk_rejection;
    IntelligentRoi intelligent_roi;
};

struct ViolationSettings
{
    std::string version;
    Enforcement enforcement;
    std::vector<LaneConfig> lane_config;
    std::vector<CameraSetup> camera_setup;
    std::vector<RemoteCameraSetup> remote_camera_setup;
    std::vector<Flash> flash;
    Video video;
    Databar databar;
    std::vector<IncidentRois> incident_rois;
    Scheduler scheduler;
    std::vector<IncidentProcessing> incident_processing;
    std::vector<LocationAlias> location_alias;
    Miscellaneous miscellaneous;
    ComputerVision computer_vision;
};
