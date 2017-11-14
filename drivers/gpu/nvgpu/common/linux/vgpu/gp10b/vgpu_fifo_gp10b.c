/*
 * Copyright (c) 2015-2017, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vgpu_fifo_gp10b.h"

void vgpu_gp10b_init_fifo_ops(struct gpu_ops *gops)
{
	/* syncpoint protection not supported yet */
	gops->fifo.resetup_ramfc = NULL;
	gops->fifo.reschedule_runlist = NULL;
}
