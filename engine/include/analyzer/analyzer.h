/* Analyzer Header */

#ifndef VMCU_ANALYZER_H
#define VMCU_ANALYZER_H

typedef struct vmcu_report vmcu_report_t;

extern vmcu_report_t* vmcu_analyze_ihex(const char *hex_file);

#endif
