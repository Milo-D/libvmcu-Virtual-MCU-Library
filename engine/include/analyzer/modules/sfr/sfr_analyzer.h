/* SFR-Analyzer Submodule Header */

#ifndef VMCU_SFR_ANALYZER_H
#define VMCU_SFR_ANALYZER_H

typedef struct vmcu_report vmcu_report_t;
typedef struct vmcu_model vmcu_model_t;

extern int vmcu_analyze_sfr(vmcu_report_t *report, vmcu_model_t *mcu);

#endif