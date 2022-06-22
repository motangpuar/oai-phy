/******************************************************************************
*
*   Copyright (c) 2020 Intel.
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*******************************************************************************/

/**
 * @brief Header file to interface implementation to ORAN FH from Application side
 * @file app_iof_fh_xran.h
 * @ingroup xran
 * @author Intel Corporation
 *
 **/

#ifndef _APP_IO_FH_H_
#define _APP_IO_FH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>

#include "config.h"

#include "xran_fh_o_du.h"
#include "xran_pkt_up.h"

#define MAX_PKT_BURST (448+4) /* 4x14x8 */
#define N_MAX_BUFFER_SEGMENT MAX_PKT_BURST

#define NUM_OF_SUBFRAME_PER_FRAME (10)

#define SW_FPGA_TOTAL_BUFFER_LEN 4*1024*1024*1024
#define SW_FPGA_SEGMENT_BUFFER_LEN 1*1024*1024*1024
#define SW_FPGA_FH_TOTAL_BUFFER_LEN 1*1024*1024*1024
#define FPGA_TO_SW_PRACH_RX_BUFFER_LEN   (8192)

extern void*  app_io_xran_handle;
extern struct xran_fh_init app_io_xran_fh_init;
extern struct xran_fh_config  app_io_xran_fh_config[XRAN_PORTS_NUM];

typedef struct
{
    uint32_t phaseFlag   :1;
    uint32_t NRARFCN     :22;
    uint32_t SULFreShift :1;
    uint32_t SULFlag     :1;
    uint32_t rsv         :7;
} FPGAPhaseCompCfg;

typedef enum {
    XRANFTHTX_OUT = 0,
    XRANFTHTX_PRB_MAP_OUT,
    XRANFTHTX_SEC_DESC_OUT,
    XRANFTHRX_IN,
    XRANFTHRX_PRB_MAP_IN,
    XRANFTHTX_SEC_DESC_IN,
    XRANFTHRACH_IN,
    XRANSRS_IN,
    XRANSRS_PRB_MAP_IN,
    XRANCP_PRB_MAP_IN_RX,
    XRANCP_PRB_MAP_IN_TX,
    XRANSRS_SEC_DESC_IN,
    MAX_SW_XRAN_INTERFACE_NUM
} SWXRANInterfaceTypeEnum;

struct xran_io_buf_ctrl {
    /* -1-this subframe is not used in current frame format
         0-this subframe can be transmitted, i.e., data is ready
          1-this subframe is waiting transmission, i.e., data is not ready
         10 - DL transmission missing deadline. When FE needs this subframe data but bValid is still 1,
        set bValid to 10.
    */
    int32_t bValid ; // when UL rx, it is subframe index.
    int32_t nSegToBeGen;
    int32_t nSegGenerated; // how many date segment are generated by DL LTE processing or received from FE
                       // -1 means that DL packet to be transmitted is not ready in BS
    int32_t nSegTransferred; // number of data segments has been transmitted or received
    struct rte_mbuf *pData[N_MAX_BUFFER_SEGMENT]; // point to DPDK allocated memory pool
    struct xran_buffer_list sBufferList;
};

struct xran_io_shared_ctrl {
    enum xran_input_byte_order byteOrder; /* Order of bytes in int16_t in buffer. Big or little endian */
    enum xran_input_i_q_order  iqOrder; /* order of IQs in the buffer */

    /* io struct */
    struct xran_io_buf_ctrl sFrontHaulTxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_io_buf_ctrl sFrontHaulTxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_io_buf_ctrl sFrontHaulRxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_io_buf_ctrl sFrontHaulRxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_io_buf_ctrl sFHPrachRxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_io_buf_ctrl sFHPrachRxBbuIoBufCtrlDecomp[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];    

    /* Cat B */
    struct xran_io_buf_ctrl sFHSrsRxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANT_ARRAY_ELM_NR];
    struct xran_io_buf_ctrl sFHSrsRxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANT_ARRAY_ELM_NR];

    struct xran_io_buf_ctrl sFHCpRxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_io_buf_ctrl sFHCpTxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];

    /* buffers lists */
    struct xran_flat_buffer sFrontHaulTxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR][XRAN_NUM_OF_SYMBOL_PER_SLOT];
    struct xran_flat_buffer sFrontHaulTxPrbMapBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_flat_buffer sFrontHaulRxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR][XRAN_NUM_OF_SYMBOL_PER_SLOT];
    struct xran_flat_buffer sFrontHaulRxPrbMapBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_flat_buffer sFHPrachRxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR][XRAN_NUM_OF_SYMBOL_PER_SLOT];
    struct xran_flat_buffer sFHPrachRxBuffersDecomp[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR][XRAN_NUM_OF_SYMBOL_PER_SLOT];
    
    struct xran_flat_buffer sFrontHaulCpRxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_flat_buffer sFrontHaulCpTxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];

    /* Cat B SRS buffers */
    struct xran_flat_buffer sFHSrsRxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANT_ARRAY_ELM_NR][XRAN_MAX_NUM_OF_SRS_SYMBOL_PER_SLOT];
    struct xran_flat_buffer sFHSrsRxPrbMapBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANT_ARRAY_ELM_NR];

    // struct xran_flat_buffer sFHCpRxPrbMapBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
};

struct bbu_xran_io_if {
    void*    nInstanceHandle[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR]; /**< instance per ORAN port per CC */
    uint32_t nBufPoolIndex[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR][MAX_SW_XRAN_INTERFACE_NUM];  /**< unique buffer pool */
    uint16_t nInstanceNum[XRAN_PORTS_NUM]; /**< instance is equivalent to CC */

    uint16_t DynamicSectionEna;
    uint16_t DynamicSectionEnaUL;
    uint32_t nPhaseCompFlag;
    uint32_t xranModCompEna;
    uint32_t xranCompMethod;
    uint32_t iqWidth;
    uint32_t mtu;

    int32_t  bbu_offload;         /**< enable packet handling on BBU cores */

    int32_t num_o_ru;
    int32_t num_cc_per_port[XRAN_PORTS_NUM];
    int32_t map_cell_id2port[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR];

    struct xran_io_shared_ctrl ioCtrl[XRAN_PORTS_NUM]; /**< for each O-RU port */

    struct xran_cb_tag  RxCbTag[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR];
    struct xran_cb_tag  PrachCbTag[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR];
    struct xran_cb_tag  SrsCbTag[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR];
    struct xran_cb_tag  BfwCbTag[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR];
};

struct bbu_xran_io_if* app_io_xran_if_alloc(void);
struct bbu_xran_io_if* app_io_xran_if_get(void);
void app_io_xran_if_free(void);
struct xran_io_shared_ctrl * app_io_xran_if_ctrl_get(uint32_t o_xu_id);
int32_t app_io_xran_sfidx_get(uint8_t nNrOfSlotInSf);

int32_t app_io_xran_interface(uint32_t o_xu_id, RuntimeConfig *p_o_xu_cfg, UsecaseConfig* p_use_cfg, struct xran_fh_init* p_xran_fh_init);
int32_t app_io_xran_iq_content_init(uint32_t o_xu_id, RuntimeConfig *p_o_xu_cfg);
int32_t app_io_xran_iq_content_get(uint32_t o_xu_id, RuntimeConfig *p_o_xu_cfg);
int32_t app_io_xran_eAxCid_conf_set(struct xran_eaxcid_config *p_eAxC_cfg, RuntimeConfig * p_s_cfg);
int32_t app_io_xran_fh_config_init(UsecaseConfig* p_use_cfg,  RuntimeConfig* p_o_xu_cfg, struct xran_fh_init* p_xran_fh_init, struct xran_fh_config*  p_xran_fh_cfg);
int32_t app_io_xran_fh_init_init(UsecaseConfig* p_use_cfg,  RuntimeConfig* p_o_xu_cfg, struct xran_fh_init* p_xran_fh_init);
int32_t app_io_xran_buffers_max_sz_set (RuntimeConfig* p_o_xu_cfg);

int32_t app_io_xran_dl_post_func(uint16_t nCellIdx, uint32_t nSfIdx, uint32_t nSymMask, uint32_t nAntStart, uint32_t nAntNum);

int32_t app_io_xran_dl_tti_call_back(void * param);
int32_t app_io_xran_ul_half_slot_call_back(void * param);
int32_t app_io_xran_ul_full_slot_call_back(void * param);
int32_t app_io_xran_ul_custom_sym_call_back(void * param, struct xran_sense_of_time* time);

int32_t app_io_xran_map_cellid_to_port(struct bbu_xran_io_if * p_xran_io, uint32_t cell_id, uint32_t *ret_cc_id);

int32_t app_io_xran_iq_content_init_cp_tx(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                  struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                  int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

int32_t app_io_xran_iq_content_init_cp_rx(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                  struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                  int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

int32_t app_io_xran_iq_content_init_up_tx(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                  struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                  int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

int32_t app_io_xran_iq_content_init_up_prach(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                    struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                    int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

int32_t app_io_xran_iq_content_init_up_srs(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                  struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                  int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

int32_t app_io_xran_iq_content_get_up_rx(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                  struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                  int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

int32_t app_io_xran_iq_content_get_up_prach(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                  struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                  int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

int32_t app_io_xran_iq_content_get_up_srs(uint8_t  appMode, struct xran_fh_config  *pXranConf,
                                  struct bbu_xran_io_if *psBbuIo, struct xran_io_shared_ctrl *psIoCtrl, struct o_xu_buffers * p_iq,
                                  int32_t cc_id, int32_t ant_id, int32_t sym_id, int32_t tti, int32_t flowId);

void app_io_xran_if_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* _APP_IO_FH_H_ */

