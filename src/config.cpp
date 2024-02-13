#include <configs.h>



ConfigParser::ConfigParser(const std::string& filename)
	: reader(filename), writer(filename)
{
	writer.setConfig(reader.getConfig());
}

ConfigReader& ConfigParser::getReader() { return reader; }
ConfigWriter& ConfigParser::getWriter() { return writer; }