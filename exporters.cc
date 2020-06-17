#include <cstdlib>
#include <iostream>

#include "opencensus/exporters/trace/stackdriver/stackdriver_exporter.h"
#include "opencensus/exporters/trace/stdout/stdout_exporter.h"

void RegisterExporters() {
	opencensus::exporters::trace::StdoutExporter::Register();

	const char*  project_id = getenv("STACKDRIVER_PROJECT_ID");
	if (project_id == nullptr) {
		std::cerr << "The STACKDRIVER_PROJECT_ID environment variable is not set: "
	             "not exporting to Stackdriver.\n";
	} else {
		std::cout << "RegisterStackdriverExporters:\n";
		std::cout << "  project_id = \"" << project_id << "\"\n";

		opencensus::exporters::trace::StackdriverOptions trace_opts;
		trace_opts.project_id = project_id;
		opencensus::exporters::trace::StackdriverExporter::Register(
			std::move(trace_opts));
	}
}