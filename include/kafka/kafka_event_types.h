#pragma once
#include <string>

namespace KafkaEvents {
	static const std::string EVENT_CONFIG_DONE = "event_config_done";
	static const std::string EVENT_INIT_DONE = "event_init_done";
	static const std::string EVENT_SYSTEM_SHUTDOWN = "event_system_shutdown";
	static const std::string EVENT_SERVICE_SHUTDOWN = "event_service_shutdown";
	static const std::string EVENT_FRAME_COMPLETE = "event_frame_complete";
	static const std::string EVENT_FRAME_INFORMATION = "event_frame_information";
	static const std::string EVENT_FRAME_READ_COMPLETE = "event_frame_read_complete";
	static const std::string EVENT_FRAME_WRITE_COMPLETE = "event_frame_write_complete";
	static const std::string EVENT_FRAME_ARRIVED = "event_frame_arrived";
	static const std::string EVENT_PROCESSING_DONE = "event_processing_done";
	static const std::string EVENT_TRACKING_DATA_READY = "event_tracking_data_ready";
	static const std::string EVENT_TRACKING_DATA = "event_tracking_data";
	static const std::string EVENT_ERROR = "event_error";
	static const std::string EVENT_INFERENCE_STARTED = "event_inference_started";
	static const std::string EVENT_INFERENCE_COMPLETE = "event_inference_complete";
	static const std::string EVENT_PONG = "event_pong";
	static const std::string EVENT_PING = "event_ping";
};
