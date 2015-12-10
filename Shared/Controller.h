#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "GPIO.h"

typedef volatile unsigned int* M4_DMA;

//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
// SECTION: UART                                                                         |
//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+

//---------------------------------------------------------------------------------------+
// UART registers (UART0)                                                                |
//---------------------------------------------------------------------------------------+
#define UART0_DR_R              (*((M4_DMA)0x4000C000))
#define UART0_RSR_R             (*((M4_DMA)0x4000C004))
#define UART0_ECR_R             (*((M4_DMA)0x4000C004))
#define UART0_FR_R              (*((M4_DMA)0x4000C018))
#define UART0_ILPR_R            (*((M4_DMA)0x4000C020))
#define UART0_IBRD_R            (*((M4_DMA)0x4000C024))
#define UART0_FBRD_R            (*((M4_DMA)0x4000C028))
#define UART0_LCRH_R            (*((M4_DMA)0x4000C02C))
#define UART0_CTL_R             (*((M4_DMA)0x4000C030))
#define UART0_IFLS_R            (*((M4_DMA)0x4000C034))
#define UART0_IM_R              (*((M4_DMA)0x4000C038))
#define UART0_RIS_R             (*((M4_DMA)0x4000C03C))
#define UART0_MIS_R             (*((M4_DMA)0x4000C040))
#define UART0_ICR_R             (*((M4_DMA)0x4000C044))

//---------------------------------------------------------------------------------------+
// UART registers (UART1)                                                                |
//---------------------------------------------------------------------------------------+
#define UART1_DR_R              (*((M4_DMA)0x4000D000))
#define UART1_RSR_R             (*((M4_DMA)0x4000D004))
#define UART1_ECR_R             (*((M4_DMA)0x4000D004))
#define UART1_FR_R              (*((M4_DMA)0x4000D018))
#define UART1_ILPR_R            (*((M4_DMA)0x4000D020))
#define UART1_IBRD_R            (*((M4_DMA)0x4000D024))
#define UART1_FBRD_R            (*((M4_DMA)0x4000D028))
#define UART1_LCRH_R            (*((M4_DMA)0x4000D02C))
#define UART1_CTL_R             (*((M4_DMA)0x4000D030))
#define UART1_IFLS_R            (*((M4_DMA)0x4000D034))
#define UART1_IM_R              (*((M4_DMA)0x4000D038))
#define UART1_RIS_R             (*((M4_DMA)0x4000D03C))
#define UART1_MIS_R             (*((M4_DMA)0x4000D040))
#define UART1_ICR_R             (*((M4_DMA)0x4000D044))
#define UART1_CC_R              (*((M4_DMA)0x4000DFC8))

//---------------------------------------------------------------------------------------+
// UART registers (UART2)                                                                |
//---------------------------------------------------------------------------------------+
#define UART2_DR_R              (*((M4_DMA)0x4000E000))
#define UART2_RSR_R             (*((M4_DMA)0x4000E004))
#define UART2_ECR_R             (*((M4_DMA)0x4000E004))
#define UART2_FR_R              (*((M4_DMA)0x4000E018))
#define UART2_ILPR_R            (*((M4_DMA)0x4000E020))
#define UART2_IBRD_R            (*((M4_DMA)0x4000E024))
#define UART2_FBRD_R            (*((M4_DMA)0x4000E028))
#define UART2_LCRH_R            (*((M4_DMA)0x4000E02C))
#define UART2_CTL_R             (*((M4_DMA)0x4000E030))
#define UART2_IFLS_R            (*((M4_DMA)0x4000E034))
#define UART2_IM_R              (*((M4_DMA)0x4000E038))
#define UART2_RIS_R             (*((M4_DMA)0x4000E03C))
#define UART2_MIS_R             (*((M4_DMA)0x4000E040))
#define UART2_ICR_R             (*((M4_DMA)0x4000E044))
#define UART2_CC_R              (*((M4_DMA)0x4000EFC8))

//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
// SECTION: Timers                                                                       |
//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+

//---------------------------------------------------------------------------------------+
// Timer registers (TIMER0)                                                              |
//---------------------------------------------------------------------------------------+
#define TIMER0_CFG_R            (*((M4_DMA)0x40030000))
#define TIMER0_TAMR_R           (*((M4_DMA)0x40030004))
#define TIMER0_TBMR_R           (*((M4_DMA)0x40030008))
#define TIMER0_CTL_R            (*((M4_DMA)0x4003000C))
#define TIMER0_IMR_R            (*((M4_DMA)0x40030018))
#define TIMER0_RIS_R            (*((M4_DMA)0x4003001C))
#define TIMER0_MIS_R            (*((M4_DMA)0x40030020))
#define TIMER0_ICR_R            (*((M4_DMA)0x40030024))
#define TIMER0_TAILR_R          (*((M4_DMA)0x40030028))
#define TIMER0_TBILR_R          (*((M4_DMA)0x4003002C))
#define TIMER0_TAMATCHR_R       (*((M4_DMA)0x40030030))
#define TIMER0_TBMATCHR_R       (*((M4_DMA)0x40030034))
#define TIMER0_TAPR_R           (*((M4_DMA)0x40030038))
#define TIMER0_TBPR_R           (*((M4_DMA)0x4003003C))
#define TIMER0_TAPMR_R          (*((M4_DMA)0x40030040))
#define TIMER0_TBPMR_R          (*((M4_DMA)0x40030044))
#define TIMER0_TAR_R            (*((M4_DMA)0x40030048))
#define TIMER0_TBR_R            (*((M4_DMA)0x4003004C))

//---------------------------------------------------------------------------------------+
// Timer registers (TIMER1)                                                              |
//---------------------------------------------------------------------------------------+
#define TIMER1_CFG_R            (*((M4_DMA)0x40031000))
#define TIMER1_TAMR_R           (*((M4_DMA)0x40031004))
#define TIMER1_TBMR_R           (*((M4_DMA)0x40031008))
#define TIMER1_CTL_R            (*((M4_DMA)0x4003100C))
#define TIMER1_IMR_R            (*((M4_DMA)0x40031018))
#define TIMER1_RIS_R            (*((M4_DMA)0x4003101C))
#define TIMER1_MIS_R            (*((M4_DMA)0x40031020))
#define TIMER1_ICR_R            (*((M4_DMA)0x40031024))
#define TIMER1_TAILR_R          (*((M4_DMA)0x40031028))
#define TIMER1_TBILR_R          (*((M4_DMA)0x4003102C))
#define TIMER1_TAMATCHR_R       (*((M4_DMA)0x40031030))
#define TIMER1_TBMATCHR_R       (*((M4_DMA)0x40031034))
#define TIMER1_TAPR_R           (*((M4_DMA)0x40031038))
#define TIMER1_TBPR_R           (*((M4_DMA)0x4003103C))
#define TIMER1_TAPMR_R          (*((M4_DMA)0x40031040))
#define TIMER1_TBPMR_R          (*((M4_DMA)0x40031044))
#define TIMER1_TAR_R            (*((M4_DMA)0x40031048))
#define TIMER1_TBR_R            (*((M4_DMA)0x4003104C))

//---------------------------------------------------------------------------------------+
// Timer registers (TIMER2)                                                              |
//---------------------------------------------------------------------------------------+
#define TIMER2_CFG_R            (*((M4_DMA)0x40032000))
#define TIMER2_TAMR_R           (*((M4_DMA)0x40032004))
#define TIMER2_TBMR_R           (*((M4_DMA)0x40032008))
#define TIMER2_CTL_R            (*((M4_DMA)0x4003200C))
#define TIMER2_IMR_R            (*((M4_DMA)0x40032018))
#define TIMER2_RIS_R            (*((M4_DMA)0x4003201C))
#define TIMER2_MIS_R            (*((M4_DMA)0x40032020))
#define TIMER2_ICR_R            (*((M4_DMA)0x40032024))
#define TIMER2_TAILR_R          (*((M4_DMA)0x40032028))
#define TIMER2_TBILR_R          (*((M4_DMA)0x4003202C))
#define TIMER2_TAMATCHR_R       (*((M4_DMA)0x40032030))
#define TIMER2_TBMATCHR_R       (*((M4_DMA)0x40032034))
#define TIMER2_TAPR_R           (*((M4_DMA)0x40032038))
#define TIMER2_TBPR_R           (*((M4_DMA)0x4003203C))
#define TIMER2_TAPMR_R          (*((M4_DMA)0x40032040))
#define TIMER2_TBPMR_R          (*((M4_DMA)0x40032044))
#define TIMER2_TAR_R            (*((M4_DMA)0x40032048))
#define TIMER2_TBR_R            (*((M4_DMA)0x4003204C))

//---------------------------------------------------------------------------------------+
// Timer registers (TIMER3)                                                              |
//---------------------------------------------------------------------------------------+
#define TIMER3_CFG_R            (*((M4_DMA)0x40033000))
#define TIMER3_TAMR_R           (*((M4_DMA)0x40033004))
#define TIMER3_TBMR_R           (*((M4_DMA)0x40033008))
#define TIMER3_CTL_R            (*((M4_DMA)0x4003300C))
#define TIMER3_IMR_R            (*((M4_DMA)0x40033018))
#define TIMER3_RIS_R            (*((M4_DMA)0x4003301C))
#define TIMER3_MIS_R            (*((M4_DMA)0x40033020))
#define TIMER3_ICR_R            (*((M4_DMA)0x40033024))
#define TIMER3_TAILR_R          (*((M4_DMA)0x40033028))
#define TIMER3_TBILR_R          (*((M4_DMA)0x4003302C))
#define TIMER3_TAMATCHR_R       (*((M4_DMA)0x40033030))
#define TIMER3_TBMATCHR_R       (*((M4_DMA)0x40033034))
#define TIMER3_TAPR_R           (*((M4_DMA)0x40033038))
#define TIMER3_TBPR_R           (*((M4_DMA)0x4003303C))
#define TIMER3_TAPMR_R          (*((M4_DMA)0x40033040))
#define TIMER3_TBPMR_R          (*((M4_DMA)0x40033044))
#define TIMER3_TAR_R            (*((M4_DMA)0x40033048))
#define TIMER3_TBR_R            (*((M4_DMA)0x4003304C))

//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
// SECTION: NVIC                                                                         |
//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+

//---------------------------------------------------------------------------------------+
// NVIC registers                                                                        |
//---------------------------------------------------------------------------------------+
#define NVIC_INT_TYPE_R         (*((M4_DMA)0xE000E004))
#define NVIC_ST_CTRL_R          (*((M4_DMA)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((M4_DMA)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((M4_DMA)0xE000E018))
#define NVIC_ST_CAL_R           (*((M4_DMA)0xE000E01C))
#define NVIC_EN0_R              (*((M4_DMA)0xE000E100))
#define NVIC_EN1_R              (*((M4_DMA)0xE000E104))
#define NVIC_DIS0_R             (*((M4_DMA)0xE000E180))
#define NVIC_DIS1_R             (*((M4_DMA)0xE000E184))
#define NVIC_PEND0_R            (*((M4_DMA)0xE000E200))
#define NVIC_PEND1_R            (*((M4_DMA)0xE000E204))
#define NVIC_UNPEND0_R          (*((M4_DMA)0xE000E280))
#define NVIC_UNPEND1_R          (*((M4_DMA)0xE000E284))
#define NVIC_ACTIVE0_R          (*((M4_DMA)0xE000E300))
#define NVIC_ACTIVE1_R          (*((M4_DMA)0xE000E304))
#define NVIC_PRI0_R             (*((M4_DMA)0xE000E400))
#define NVIC_PRI1_R             (*((M4_DMA)0xE000E404))
#define NVIC_PRI2_R             (*((M4_DMA)0xE000E408))
#define NVIC_PRI3_R             (*((M4_DMA)0xE000E40C))
#define NVIC_PRI4_R             (*((M4_DMA)0xE000E410))
#define NVIC_PRI5_R             (*((M4_DMA)0xE000E414))
#define NVIC_PRI6_R             (*((M4_DMA)0xE000E418))
#define NVIC_PRI7_R             (*((M4_DMA)0xE000E41C))
#define NVIC_PRI8_R             (*((M4_DMA)0xE000E420))
#define NVIC_PRI9_R             (*((M4_DMA)0xE000E424))
#define NVIC_PRI10_R            (*((M4_DMA)0xE000E428))
#define NVIC_CPUID_R            (*((M4_DMA)0xE000ED00))
#define NVIC_INT_CTRL_R         (*((M4_DMA)0xE000ED04))
#define NVIC_VTABLE_R           (*((M4_DMA)0xE000ED08))
#define NVIC_APINT_R            (*((M4_DMA)0xE000ED0C))
#define NVIC_SYS_CTRL_R         (*((M4_DMA)0xE000ED10))
#define NVIC_CFG_CTRL_R         (*((M4_DMA)0xE000ED14))
#define NVIC_SYS_PRI1_R         (*((M4_DMA)0xE000ED18))
#define NVIC_SYS_PRI2_R         (*((M4_DMA)0xE000ED1C))
#define NVIC_SYS_PRI3_R         (*((M4_DMA)0xE000ED20))
#define NVIC_SYS_HND_CTRL_R     (*((M4_DMA)0xE000ED24))
#define NVIC_FAULT_STAT_R       (*((M4_DMA)0xE000ED28))
#define NVIC_HFAULT_STAT_R      (*((M4_DMA)0xE000ED2C))
#define NVIC_DEBUG_STAT_R       (*((M4_DMA)0xE000ED30))
#define NVIC_MM_ADDR_R          (*((M4_DMA)0xE000ED34))
#define NVIC_FAULT_ADDR_R       (*((M4_DMA)0xE000ED38))
#define NVIC_MPU_TYPE_R         (*((M4_DMA)0xE000ED90))
#define NVIC_MPU_CTRL_R         (*((M4_DMA)0xE000ED94))
#define NVIC_MPU_NUMBER_R       (*((M4_DMA)0xE000ED98))
#define NVIC_MPU_BASE_R         (*((M4_DMA)0xE000ED9C))
#define NVIC_MPU_ATTR_R         (*((M4_DMA)0xE000EDA0))
#define NVIC_DBG_CTRL_R         (*((M4_DMA)0xE000EDF0))
#define NVIC_DBG_XFER_R         (*((M4_DMA)0xE000EDF4))
#define NVIC_DBG_DATA_R         (*((M4_DMA)0xE000EDF8))
#define NVIC_DBG_INT_R          (*((M4_DMA)0xE000EDFC))
#define NVIC_SW_TRIG_R          (*((M4_DMA)0xE000EF00))

//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
// SECTION: GPIO                                                                         |
//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
#define GPIO_UNLOCK             0x4C4F434B

//---------------------------------------------------------------------------------------+
// GPIO registers (PORTA)                                                                |
//---------------------------------------------------------------------------------------+
#define GPIO_PORTA_DATA_R       (*((M4_DMA)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((M4_DMA)0x40004400))
#define GPIO_PORTA_IS_R         (*((M4_DMA)0x40004404))
#define GPIO_PORTA_IBE_R        (*((M4_DMA)0x40004408))
#define GPIO_PORTA_IEV_R        (*((M4_DMA)0x4000440C))
#define GPIO_PORTA_IM_R         (*((M4_DMA)0x40004410))
#define GPIO_PORTA_RIS_R        (*((M4_DMA)0x40004414))
#define GPIO_PORTA_MIS_R        (*((M4_DMA)0x40004418))
#define GPIO_PORTA_ICR_R        (*((M4_DMA)0x4000441C))
#define GPIO_PORTA_AFSEL_R      (*((M4_DMA)0x40004420))
#define GPIO_PORTA_DR2R_R       (*((M4_DMA)0x40004500))
#define GPIO_PORTA_DR4R_R       (*((M4_DMA)0x40004504))
#define GPIO_PORTA_DR8R_R       (*((M4_DMA)0x40004508))
#define GPIO_PORTA_ODR_R        (*((M4_DMA)0x4000450C))
#define GPIO_PORTA_PUR_R        (*((M4_DMA)0x40004510))
#define GPIO_PORTA_PDR_R        (*((M4_DMA)0x40004514))
#define GPIO_PORTA_SLR_R        (*((M4_DMA)0x40004518))
#define GPIO_PORTA_DEN_R        (*((M4_DMA)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((M4_DMA)0x40004520))
#define GPIO_PORTA_CR_R         (*((M4_DMA)0x40004524))
#define GPIO_PORTA_PCTL_R       (*((M4_DMA)0x4000452C))

//---------------------------------------------------------------------------------------+
// GPIO registers (PORTB)                                                                |
//---------------------------------------------------------------------------------------+
#define GPIO_PORTB_DATA_R       (*((M4_DMA)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((M4_DMA)0x40005400))
#define GPIO_PORTB_IS_R         (*((M4_DMA)0x40005404))
#define GPIO_PORTB_IBE_R        (*((M4_DMA)0x40005408))
#define GPIO_PORTB_IEV_R        (*((M4_DMA)0x4000540C))
#define GPIO_PORTB_IM_R         (*((M4_DMA)0x40005410))
#define GPIO_PORTB_RIS_R        (*((M4_DMA)0x40005414))
#define GPIO_PORTB_MIS_R        (*((M4_DMA)0x40005418))
#define GPIO_PORTB_ICR_R        (*((M4_DMA)0x4000541C))
#define GPIO_PORTB_AFSEL_R      (*((M4_DMA)0x40005420))
#define GPIO_PORTB_DR2R_R       (*((M4_DMA)0x40005500))
#define GPIO_PORTB_DR4R_R       (*((M4_DMA)0x40005504))
#define GPIO_PORTB_DR8R_R       (*((M4_DMA)0x40005508))
#define GPIO_PORTB_ODR_R        (*((M4_DMA)0x4000550C))
#define GPIO_PORTB_PUR_R        (*((M4_DMA)0x40005510))
#define GPIO_PORTB_PDR_R        (*((M4_DMA)0x40005514))
#define GPIO_PORTB_SLR_R        (*((M4_DMA)0x40005518))
#define GPIO_PORTB_DEN_R        (*((M4_DMA)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((M4_DMA)0x40005520))
#define GPIO_PORTB_CR_R         (*((M4_DMA)0x40005524))
#define GPIO_PORTB_PCTL_R       (*((M4_DMA)0x4000552C))

//---------------------------------------------------------------------------------------+
// GPIO registers (PORTC)                                                                |
//---------------------------------------------------------------------------------------+
#define GPIO_PORTC_DATA_R       (*((M4_DMA)0x400063FC))
#define GPIO_PORTC_DIR_R        (*((M4_DMA)0x40006400))
#define GPIO_PORTC_IS_R         (*((M4_DMA)0x40006404))
#define GPIO_PORTC_IBE_R        (*((M4_DMA)0x40006408))
#define GPIO_PORTC_IEV_R        (*((M4_DMA)0x4000640C))
#define GPIO_PORTC_IM_R         (*((M4_DMA)0x40006410))
#define GPIO_PORTC_RIS_R        (*((M4_DMA)0x40006414))
#define GPIO_PORTC_MIS_R        (*((M4_DMA)0x40006418))
#define GPIO_PORTC_ICR_R        (*((M4_DMA)0x4000641C))
#define GPIO_PORTC_AFSEL_R      (*((M4_DMA)0x40006420))
#define GPIO_PORTC_DR2R_R       (*((M4_DMA)0x40006500))
#define GPIO_PORTC_DR4R_R       (*((M4_DMA)0x40006504))
#define GPIO_PORTC_DR8R_R       (*((M4_DMA)0x40006508))
#define GPIO_PORTC_ODR_R        (*((M4_DMA)0x4000650C))
#define GPIO_PORTC_PUR_R        (*((M4_DMA)0x40006510))
#define GPIO_PORTC_PDR_R        (*((M4_DMA)0x40006514))
#define GPIO_PORTC_SLR_R        (*((M4_DMA)0x40006518))
#define GPIO_PORTC_DEN_R        (*((M4_DMA)0x4000651C))
#define GPIO_PORTC_LOCK_R       (*((M4_DMA)0x40006520))
#define GPIO_PORTC_CR_R         (*((M4_DMA)0x40006524))
#define GPIO_PORTC_PCTL_R       (*((M4_DMA)0x4000652C))

//---------------------------------------------------------------------------------------+
// GPIO registers (PORTD)                                                                |
//---------------------------------------------------------------------------------------+
#define GPIO_PORTD_DATA_R       (*((M4_DMA)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((M4_DMA)0x40007400))
#define GPIO_PORTD_IS_R         (*((M4_DMA)0x40007404))
#define GPIO_PORTD_IBE_R        (*((M4_DMA)0x40007408))
#define GPIO_PORTD_IEV_R        (*((M4_DMA)0x4000740C))
#define GPIO_PORTD_IM_R         (*((M4_DMA)0x40007410))
#define GPIO_PORTD_RIS_R        (*((M4_DMA)0x40007414))
#define GPIO_PORTD_MIS_R        (*((M4_DMA)0x40007418))
#define GPIO_PORTD_ICR_R        (*((M4_DMA)0x4000741C))
#define GPIO_PORTD_AFSEL_R      (*((M4_DMA)0x40007420))
#define GPIO_PORTD_DR2R_R       (*((M4_DMA)0x40007500))
#define GPIO_PORTD_DR4R_R       (*((M4_DMA)0x40007504))
#define GPIO_PORTD_DR8R_R       (*((M4_DMA)0x40007508))
#define GPIO_PORTD_ODR_R        (*((M4_DMA)0x4000750C))
#define GPIO_PORTD_PUR_R        (*((M4_DMA)0x40007510))
#define GPIO_PORTD_PDR_R        (*((M4_DMA)0x40007514))
#define GPIO_PORTD_SLR_R        (*((M4_DMA)0x40007518))
#define GPIO_PORTD_DEN_R        (*((M4_DMA)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((M4_DMA)0x40007520))
#define GPIO_PORTD_CR_R         (*((M4_DMA)0x40007524))
#define GPIO_PORTD_PCTL_R       (*((M4_DMA)0x4000752C))

//---------------------------------------------------------------------------------------+
// GPIO registers (PORTE)                                                                |
//---------------------------------------------------------------------------------------+
#define GPIO_PORTE_DATA_R       (*((M4_DMA)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((M4_DMA)0x40024400))
#define GPIO_PORTE_IS_R         (*((M4_DMA)0x40024404))
#define GPIO_PORTE_IBE_R        (*((M4_DMA)0x40024408))
#define GPIO_PORTE_IEV_R        (*((M4_DMA)0x4002440C))
#define GPIO_PORTE_IM_R         (*((M4_DMA)0x40024410))
#define GPIO_PORTE_RIS_R        (*((M4_DMA)0x40024414))
#define GPIO_PORTE_MIS_R        (*((M4_DMA)0x40024418))
#define GPIO_PORTE_ICR_R        (*((M4_DMA)0x4002441C))
#define GPIO_PORTE_AFSEL_R      (*((M4_DMA)0x40024420))
#define GPIO_PORTE_DR2R_R       (*((M4_DMA)0x40024500))
#define GPIO_PORTE_DR4R_R       (*((M4_DMA)0x40024504))
#define GPIO_PORTE_DR8R_R       (*((M4_DMA)0x40024508))
#define GPIO_PORTE_ODR_R        (*((M4_DMA)0x4002450C))
#define GPIO_PORTE_PUR_R        (*((M4_DMA)0x40024510))
#define GPIO_PORTE_PDR_R        (*((M4_DMA)0x40024514))
#define GPIO_PORTE_SLR_R        (*((M4_DMA)0x40024518))
#define GPIO_PORTE_DEN_R        (*((M4_DMA)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((M4_DMA)0x40024520))
#define GPIO_PORTE_CR_R         (*((M4_DMA)0x40024524))
#define GPIO_PORTE_PCTL_R       (*((M4_DMA)0x4002452C))

//---------------------------------------------------------------------------------------+
// GPIO registers (PORTF)                                                                |
//---------------------------------------------------------------------------------------+
#define GPIO_PORTF_DATA_R       (*((M4_DMA)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((M4_DMA)0x40025400))
#define GPIO_PORTF_IS_R         (*((M4_DMA)0x40025404))
#define GPIO_PORTF_IBE_R        (*((M4_DMA)0x40025408))
#define GPIO_PORTF_IEV_R        (*((M4_DMA)0x4002540C))
#define GPIO_PORTF_IM_R         (*((M4_DMA)0x40025410))
#define GPIO_PORTF_RIS_R        (*((M4_DMA)0x40025414))
#define GPIO_PORTF_MIS_R        (*((M4_DMA)0x40025418))
#define GPIO_PORTF_ICR_R        (*((M4_DMA)0x4002541C))
#define GPIO_PORTF_AFSEL_R      (*((M4_DMA)0x40025420))
#define GPIO_PORTF_DR2R_R       (*((M4_DMA)0x40025500))
#define GPIO_PORTF_DR4R_R       (*((M4_DMA)0x40025504))
#define GPIO_PORTF_DR8R_R       (*((M4_DMA)0x40025508))
#define GPIO_PORTF_ODR_R        (*((M4_DMA)0x4002550C))
#define GPIO_PORTF_PUR_R        (*((M4_DMA)0x40025510))
#define GPIO_PORTF_PDR_R        (*((M4_DMA)0x40025514))
#define GPIO_PORTF_SLR_R        (*((M4_DMA)0x40025518))
#define GPIO_PORTF_DEN_R        (*((M4_DMA)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((M4_DMA)0x40025520))
#define GPIO_PORTF_CR_R         (*((M4_DMA)0x40025524))
#define GPIO_PORTF_PCTL_R       (*((M4_DMA)0x4002552C))

//---------------------------------------------------------------------------------------+
// GPIO Bit-Banded Addresses                                                             |
//---------------------------------------------------------------------------------------+
// PA: 0x42000000 + 32*0x43FC = 0x42087F80
#define BAND_GPIO_PA2           (*((volatile unsigned int *)0x42087F88))
#define BAND_GPIO_PA3           (*((volatile unsigned int *)0x42087F8C))
#define BAND_GPIO_PA4           (*((volatile unsigned int *)0x42087F90))
#define BAND_GPIO_PA5           (*((volatile unsigned int *)0x42087F94))
#define BAND_GPIO_PA6           (*((volatile unsigned int *)0x42087F98))
#define BAND_GPIO_PA7           (*((volatile unsigned int *)0x42087F9C))

// PB: 0x42000000 + 32*0x53FC = 0x420A7F80
#define BAND_GPIO_PB0           (*((volatile unsigned int *)0x420A7F80))
#define BAND_GPIO_PB1           (*((volatile unsigned int *)0x420A7F84))
#define BAND_GPIO_PB2           (*((volatile unsigned int *)0x420A7F88))
#define BAND_GPIO_PB3           (*((volatile unsigned int *)0x420A7F8C))
#define BAND_GPIO_PB4           (*((volatile unsigned int *)0x420A7F90))
#define BAND_GPIO_PB5           (*((volatile unsigned int *)0x420A7F94))
#define BAND_GPIO_PB6           (*((volatile unsigned int *)0x420A7F98))
#define BAND_GPIO_PB7           (*((volatile unsigned int *)0x420A7F9C))

// PC: 0x42000000 + 32*0x63FC = 0x420C7F80
#define BAND_GPIO_PC4           (*((volatile unsigned int *)0x420C7F90))
#define BAND_GPIO_PC5           (*((volatile unsigned int *)0x420C7F94))
#define BAND_GPIO_PC6           (*((volatile unsigned int *)0x420C7F98))
#define BAND_GPIO_PC7           (*((volatile unsigned int *)0x420C7F9C))

// PD: 0x42000000 + 32*0x73FC = 0x420E7F80
#define BAND_GPIO_PD2           (*((volatile unsigned int *)0x420E7F88))
#define BAND_GPIO_PD3           (*((volatile unsigned int *)0x420E7F8C))
#define BAND_GPIO_PD6           (*((volatile unsigned int *)0x420E7F98))
#define BAND_GPIO_PD7           (*((volatile unsigned int *)0x420E7F9C))

// PE: 0x42000000 + 32*0x243FC = 0x42487F80
#define BAND_GPIO_PE0           (*((volatile unsigned int *)0x42487F80))
#define BAND_GPIO_PE1           (*((volatile unsigned int *)0x42487F84))
#define BAND_GPIO_PE2           (*((volatile unsigned int *)0x42487F88))
#define BAND_GPIO_PE3           (*((volatile unsigned int *)0x42487F8C))
#define BAND_GPIO_PE4           (*((volatile unsigned int *)0x42487F90))
#define BAND_GPIO_PE5           (*((volatile unsigned int *)0x42487F94))

// PF: 0x42000000 + 32*0x253FC = 0x424A7F80
#define BAND_GPIO_PF0           (*((volatile unsigned int *)0x424A7F80))
#define BAND_GPIO_PF1           (*((volatile unsigned int *)0x424A7F84))
#define BAND_GPIO_PF2           (*((volatile unsigned int *)0x424A7F88))
#define BAND_GPIO_PF3           (*((volatile unsigned int *)0x424A7F8C))
#define BAND_GPIO_PF4           (*((volatile unsigned int *)0x424A7F90))

//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
// SECTION: MISC                                                                         |
//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+

//---------------------------------------------------------------------------------------+
// Bit-Banded Addresses For On-Board Connected Features                                  |
//---------------------------------------------------------------------------------------+
#define BOARD_LED_RED           BAND_GPIO_PF1
#define BOARD_LED_BLUE          BAND_GPIO_PF2
#define BOARD_LED_GREEN         BAND_GPIO_PF3

#endif
