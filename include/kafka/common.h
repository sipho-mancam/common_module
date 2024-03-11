#pragma once

#include <glib.h>
#include <librdkafka/rdkafkacpp.h>
#include <librdkafka/rdkafka.h>

#define g_autoptr(o) o*
#define g_autofree 

void load_config_group(rd_kafka_conf_t* conf, GKeyFile* key_file, const char* group);
namespace KAFKA_KEY_TYPES {
	static const std::string KAFKA_KEY_EVENT = "event";
	static const std::string KAFKA_KEY_MESSAGE = "message";
	static const std::string KAFKA_KEY_LOG = "log";
}
