/* Analyzer Header */

#ifndef VMCU_ANALYZER_H
#define VMCU_ANALYZER_H

typedef struct vmcu_report vmcu_report_t;
typedef struct vmcu_model vmcu_model_t;

extern vmcu_report_t* vmcu_analyze_ihex(const char *hex_file, vmcu_model_t *mcu);

#endif
