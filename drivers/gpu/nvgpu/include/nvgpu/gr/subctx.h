/*
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

#ifndef NVGPU_GR_SUBCTX_H
#define NVGPU_GR_SUBCTX_H

#include <nvgpu/types.h>
#include <nvgpu/nvgpu_mem.h>

struct gk20a;
struct vm_gk20a;

struct nvgpu_gr_subctx {
	struct nvgpu_mem ctx_header;
};

struct nvgpu_gr_subctx *nvgpu_gr_subctx_alloc(struct gk20a *g,
	struct vm_gk20a *vm);
void nvgpu_gr_subctx_free(struct gk20a *g,
	struct nvgpu_gr_subctx *subctx,
	struct vm_gk20a *vm);

void nvgpu_gr_subctx_load_ctx_header(struct gk20a *g,
	struct nvgpu_gr_subctx *subctx,
	struct nvgpu_gr_ctx *gr_ctx, u64 gpu_va);

void nvgpu_gr_subctx_zcull_setup(struct gk20a *g, struct nvgpu_gr_subctx *subctx,
		struct nvgpu_gr_ctx *gr_ctx);

void nvgpu_gr_subctx_set_hwpm_mode(struct gk20a *g,
	struct nvgpu_gr_subctx *subctx, struct nvgpu_gr_ctx *gr_ctx);

void nvgpu_gr_subctx_set_patch_ctx(struct gk20a *g,
	struct nvgpu_gr_subctx *subctx, struct nvgpu_gr_ctx *gr_ctx);

void nvgpu_gr_subctx_set_preemption_buffer_va(struct gk20a *g,
	struct nvgpu_gr_subctx *subctx, struct nvgpu_gr_ctx *gr_ctx);

#endif /* NVGPU_GR_SUBCTX_H */
