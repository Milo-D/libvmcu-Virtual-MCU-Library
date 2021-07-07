/* Controlflow Analyzer Submodule Header */

#ifndef VMCU_CONTROL_FLOW_ANALYZER_H
#define VMCU_CONTROL_FLOW_ANALYZER_H

typedef struct vmcu_model vmcu_model_t;
typedef struct vmcu_report vmcu_report_t;

extern int vmcu_analyze_control_flow(vmcu_report_t *report, vmcu_model_t *mcu);

#endif
