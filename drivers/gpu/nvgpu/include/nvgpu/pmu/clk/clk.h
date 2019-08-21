/*
 * general clock structures & definitions
 *
 * Copyright (c) 2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef NVGPU_PMU_CLK_H
#define NVGPU_PMU_CLK_H

#include <nvgpu/types.h>

struct gk20a;
struct nvgpu_avfsfllobjs;
struct nvgpu_clk_domains;
struct nvgpu_clk_progs;
struct nvgpu_clk_vf_points;
struct nvgpu_clk_mclk_state;
struct nvgpu_clk_freq_controllers;
struct nvgpu_clk_freq_domain_grp;
struct nvgpu_clk_slave_freq;
struct ctrl_perf_change_seq_change_input;

struct nvgpu_clkrpc_pmucmdhandler_params {
	struct nv_pmu_clk_rpc *prpccall;
	u32 success;
};

struct nvgpu_clockentry {
	u8 vbios_clk_domain;
	u8 clk_which;
	u8 perf_index;
	u32 api_clk_domain;
};

struct nvgpu_set_fll_clk {
	u32 voltuv;
	u16 gpc2clkmhz;
	u8 current_regime_id_gpc;
	u8 target_regime_id_gpc;
	u16 sys2clkmhz;
	u8 current_regime_id_sys;
	u8 target_regime_id_sys;
	u16 xbar2clkmhz;
	u8 current_regime_id_xbar;
	u8 target_regime_id_xbar;
	u16 nvdclkmhz;
	u8 current_regime_id_nvd;
	u8 target_regime_id_nvd;
	u16 hostclkmhz;
	u8 current_regime_id_host;
	u8 target_regime_id_host;
};

struct nvgpu_clk_pmupstate {
	struct nvgpu_avfsvinobjs *avfs_vinobjs;
	struct nvgpu_avfsfllobjs *avfs_fllobjs;
	struct nvgpu_clk_domains *clk_domainobjs;
	struct nvgpu_clk_progs *clk_progobjs;
	struct nvgpu_clk_vf_points *clk_vf_pointobjs;
	struct nvgpu_clk_freq_controllers *clk_freq_controllers;
	struct nvgpu_clk_freq_domain_grp *freq_domain_grp_objs;

	/* clk_domain unit functions */
	int (*get_fll)(struct gk20a *g, struct nvgpu_set_fll_clk *setfllclk);
	int (*set_boot_fll)(struct gk20a *g);
	void (*set_p0_clks)(struct gk20a *g, u8 *gpcclk_domain,
		u32 *gpcclk_clkmhz, struct nvgpu_clk_slave_freq *vf_point,
		struct ctrl_perf_change_seq_change_input *change_input);
	struct nvgpu_clk_domain *(*clk_get_clk_domain)
			(struct nvgpu_clk_pmupstate *pclk, u8 idx);
	int (*clk_domain_clk_prog_link)(struct gk20a *g,
			struct nvgpu_clk_pmupstate *pclk);

	/* clk_vin unit functions */
	struct nvgpu_vin_device *(*clk_get_vin)
			(struct nvgpu_avfsvinobjs *pvinobjs, u8 idx);

	/* clk_fll unit functions */
	u8 (*find_regime_id)(struct gk20a *g, u32 domain, u16 clkmhz);
	int (*set_regime_id)(struct gk20a *g, u32 domain, u8 regimeid);
	int (*get_regime_id)(struct gk20a *g, u32 domain, u8 *regimeid);
	u8 (*get_fll_lut_vf_num_entries)(struct nvgpu_clk_pmupstate *pclk);
	u32 (*get_fll_lut_min_volt)(struct nvgpu_clk_pmupstate *pclk);
	u32 (*get_fll_lut_step_size)(struct nvgpu_clk_pmupstate *pclk);

	/* clk_vf_point functions */
	int (*nvgpu_clk_vf_point_cache)(struct gk20a *g);
};

void nvgpu_clkrpc_pmucmdhandler(struct gk20a *g, struct pmu_msg *msg,
		void *param, u32 status);
int nvgpu_clk_init_pmupstate(struct gk20a *g);
void nvgpu_clk_free_pmupstate(struct gk20a *g);
int nvgpu_clk_set_boot_fll_clk_tu10x(struct gk20a *g);
int nvgpu_clk_get_fll_clks(struct gk20a *g,
		struct nvgpu_set_fll_clk *setfllclk);
int nvgpu_clk_set_fll_clks(struct gk20a *g,
		struct nvgpu_set_fll_clk *setfllclk);
int nvgpu_clk_domain_freq_to_volt(struct gk20a *g, u8 clkdomain_idx,
	u32 *pclkmhz, u32 *pvoltuv, u8 railidx);
int nvgpu_clk_set_req_fll_clk_ps35(struct gk20a *g,
	struct nvgpu_clk_slave_freq *vf_point);

#endif /* NVGPU_PMU_CLK_H */