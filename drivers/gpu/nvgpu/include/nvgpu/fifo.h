/*
 * FIFO common definitions.
 *
 * Copyright (c) 2011-2019, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef NVGPU_FIFO_COMMON_H
#define NVGPU_FIFO_COMMON_H

/**
 * @file
 * @page unit-fifo Unit FIFO
 *
 * Overview
 * ========
 *
 * The FIFO unit is responsible for managing xxxxx.
 * primarily of TODO types:
 *
 *   + TODO
 *   + TODO
 *
 * The FIFO code also makes sure that all of the necessary SW and HW
 * initialization for engines, pdbma, runlist, channel and tsg subsystems
 * are taken care of before the GPU begins executing work.
 *
 * Top level FIFO Unit
 * ---------------------
 *
 * The FIFO unit TODO.
 *
 *   + include/nvgpu/fifo.h
 *   + include/nvgpu/gops_fifo.h
 *
 * Runlist
 * -------
 *
 * TODO
 *
 *   + include/nvgpu/runlist.h
 *   + include/nvgpu/gops_runlist.h
 *
 * Pbdma
 * -------
 *
 * TODO
 *
 *   + include/nvgpu/pbdma.h
 *   + include/nvgpu/pbdma_status.h
 *
 * Engines
 * -------
 *
 * TODO
 *
 *   + include/nvgpu/engines.h
 *   + include/nvgpu/engine_status.h
 *   + include/nvgpu/gops_engine.h
 *
 * Preempt
 * -------
 *
 * TODO
 *
 *   + include/nvgpu/preempt.h
 *
 * Channel
 * -------
 *
 * TODO
 *
 *   + include/nvgpu/channel.h
 *   + include/nvgpu/gops_channel.h
 *
 * Tsg
 * -------
 *
 * TODO
 *
 *   + include/nvgpu/tsg.h
 *
 * RAM
 * -------
 *
 * TODO
 *
 *   + include/nvgpu/gops_ramin.h
 *   + include/nvgpu/gops_ramfc.h
 *
 * Sync
 * ----
 *
 *   + include/nvgpu/channel_sync.h
 *   + include/nvgpu/channel_sync_syncpt.h
 *   + include/nvgpu/gops_sync.h
 *
 * Usermode
 * --------
 *
 * TODO
 *
 *   + include/nvgpu/gops_usermode.h
 *
 *
 * Data Structures
 * ===============
 *
 * The major data structures exposed to users of the FIFO unit in nvgpu relate
 * to managing Engines, Runlists, Channels and Tsgs.
 * Following is a list of these structures:
 *
 *   + struct nvgpu_fifo
 *
 *       TODO
 *
 *   + struct nvgpu_runlist_info
 *
 *       TODO
 *
 *   + struct nvgpu_engine_info
 *
 *       TODO
 *
 *   + struct nvgpu_channel
 *
 *       TODO
 *
 *   + struct nvgpu_tsg
 *
 *       TODO
 *
 * Static Design
 * =============
 *
 * Details of static design.
 *
 * Resource utilization
 * --------------------
 *
 * External APIs
 * -------------
 *
 *   + TODO
 *
 *
 * Supporting Functionality
 * ========================
 *
 * There's a fair amount of supporting functionality:
 *
 *   + TODO
 *     - TODO
 *   + TODO
 *   + TODO
 *     # TODO
 *     # TODO
 *
 * Documentation for this will be filled in!
 *
 * Dependencies
 * ------------
 *
 * Dynamic Design
 * ==============
 *
 * Use case descriptions go here. Some potentials:
 *
 *   - TODO
 *   - TODO
 *   - TODO
 *
 * Open Items
 * ==========
 *
 * Any open items can go here.
 */

#include <nvgpu/types.h>
#include <nvgpu/lock.h>
#include <nvgpu/kref.h>
#include <nvgpu/list.h>
/**
 * H/w defined value for Channel ID type
 */
#define ID_TYPE_CHANNEL			0U
/**
 * H/w defined value for Tsg ID type
 */
#define ID_TYPE_TSG			1U
/**
 * S/w defined value for unknown ID type.
 */
#define ID_TYPE_UNKNOWN			(~U32(0U))
/**
 * Invalid ID.
 */
#define INVAL_ID			(~U32(0U))
/**
 * Timeout after which ctxsw timeout interrupt (if enabled by s/w) will be
 * triggered by h/w if context fails to context switch.
 */
#define CTXSW_TIMEOUT_PERIOD_MS		100U

/** Subctx id 0 */
#define CHANNEL_INFO_VEID0		0U

struct gk20a;
struct nvgpu_engine_info;
struct nvgpu_runlist_info;
struct nvgpu_channel;
struct nvgpu_tsg;

struct nvgpu_fifo {
	/** Pointer to GPU driver struct. */
	struct gk20a *g;
	/** Number of channels supported by the h/w. */
	unsigned int num_channels;
	/** Runlist entry size in bytes as supported by h/w. */
	unsigned int runlist_entry_size;
	/** Number of runlist entries per runlist as supported by the h/w. */
	unsigned int num_runlist_entries;

	/** Number of PBDMA supported by the h/w. */
	unsigned int num_pbdma;
	/**
	 * This is the area of memory allocated by kernel to store pbdma_map for
	 * #num_pbdma supported by the chip. This area of memory is used to
	 * store pbdma map value as read from h/w register. Pbdma_map value
	 * gives a bitmask describing the runlists that the given pbdma
	 * will service. Pointer is indexed by pbdma_id starting with 0 to
	 * #num_pbdma - 1.
	 */
	u32 *pbdma_map;

	/**
	 * This is the area of memory allocated by kernel to keep information for
	 * #max_engines supported by the chip. This information is filled up
	 * with device info h/w registers' values. Pointer is indexed by
	 * engine_id defined by h/w.
	 */
	struct nvgpu_engine_info *engine_info;
	/**
	 * Total number of engines supported on the chip. This variable is
	 * updated with one of the h/w register's value defined for chip
	 * configuration related settings.
	 */
	u32 max_engines;
	/**
	 * This represents total number of active engines supported on the chip.
	 * This is calculated based on total number of available engines
	 * read from device info h/w registers. This variable can be less than
	 * or equal to #max_engines.
	 */
	u32 num_engines;
	/**
	 * This is the area of memory allocated by kernel for #max_engines
	 * supported by the chip. This is needed to map engine_id defined
	 * by s/w to engine_id defined by device info h/w registers.
	 * This area of memory is indexed by s/w defined engine_id starting
	 * with 0.
	 */
	u32 *active_engines_list;

	/**
	 * Pointers to runlists, indexed by real hw runlist_id.
	 * If a runlist is active, then runlist_info[runlist_id] points
	 * to one entry in active_runlist_info. Otherwise, it is NULL.
	 */
	struct nvgpu_runlist_info **runlist_info;
	/** Number of runlists supported by the h/w. */
	u32 max_runlists;

	/** Array of runlists that are actually in use. */
	struct nvgpu_runlist_info *active_runlist_info;
	/** Number of active runlists. */
	u32 num_runlists;

#ifdef CONFIG_DEBUG_FS
	struct {
		struct nvgpu_profile *data;
		nvgpu_atomic_t get;
		bool enabled;
		u64 *sorted;
		struct nvgpu_ref ref;
		struct nvgpu_mutex lock;
	} profile;
#endif
#ifdef CONFIG_NVGPU_USERD
	struct nvgpu_mutex userd_mutex;
	struct nvgpu_mem *userd_slabs;
	u32 num_userd_slabs;
	u32 num_channels_per_slab;
	u64 userd_gpu_va;
#endif

	/**
	 * Number of channels in use. This is incremented by one when a
	 * channel is opened and decremented by one when a channel is closed by
	 * userspace.
	 */
	unsigned int used_channels;
	/**
	 * This is the zero initialized area of memory allocated by kernel for
	 * storing channel specific data i.e. #nvgpu_channel struct info for
	 * #num_channels number of channels.
	 */
	struct nvgpu_channel *channel;
	/** List of channels available for allocation */
	struct nvgpu_list_node free_chs;
	/**
	 * Lock used to read and update #free_chs list. Channel entry is
	 * removed when a channel is openend and added back to the #free_ch list
	 * when channel is closed by userspace.
	 * This lock is also used to protect #used_channels.
	 */
	struct nvgpu_mutex free_chs_mutex;

	/** Lock used to prevent multiple recoveries. */
	struct nvgpu_mutex engines_reset_mutex;

	/** Lock used to update h/w runlist registers for submitting runlist. */
	struct nvgpu_spinlock runlist_submit_lock;

	/**
	 * This is the zero initialized area of memory allocated by kernel for
	 * storing TSG specific data i.e. #nvgpu_tsg struct info for
	 * #num_channels number of TSG.
	 */
	struct nvgpu_tsg *tsg;
	/**
	 * Lock used to read and update #nvgpu_tsg.in_use. TSG entry is
	 * in use when a TSG is openend and not in use when TSG is closed
	 * by userspace. Refer #nvgpu_tsg.in_use in tsg.h.
	 */
	struct nvgpu_mutex tsg_inuse_mutex;

	/**
	 * Pointer to a function that will be executed when FIFO support
	 * is requested to be removed. This is supposed to clean up
	 * all s/w resources used by FIFO module e.g. Channel, TSG, PBDMA,
	 * Runlist, Engines and USERD.
	 */
	void (*remove_support)(struct nvgpu_fifo *f);

	/**
	 * nvgpu_fifo_setup_sw is skipped if this flag is set to true.
	 * This gets set to true after successful completion of
	 * nvgpu_fifo_setup_sw.
	 */
	bool sw_ready;

	/** FIFO interrupt related fields. */
	struct {
		/** Share info between isrs and non-isr code. */
		struct {
			/** Lock for bottom half of isr. */
			struct nvgpu_mutex mutex;
		} isr;
		/** PBDMA interrupt specific data. */
		struct {
			/** H/w specific unrecoverable PBDMA interrupts. */
			u32 device_fatal_0;
			/**
			 * H/w specific recoverable PBDMA interrupts that are
			 * limited to channels. Fixing and clearing the
			 * interrupt will allow PBDMA to continue.
			 */
			u32 channel_fatal_0;
			/** H/w specific recoverable PBDMA interrupts. */
			u32 restartable_0;
		} pbdma;

		/** Engine interrupt specific data. */
		struct {

		} engine;


	} intr;

#ifdef CONFIG_NVGPU_DEBUGGER
	unsigned long deferred_fault_engines;
	bool deferred_reset_pending;
	struct nvgpu_mutex deferred_reset_mutex;
#endif

	/** Max number of sub context i.e. veid supported by the h/w. */
	u32 max_subctx_count;
	/** Used for vgpu. */
	u32 channel_base;
};

/**
 * @brief Initialize FIFO software context.
 *
 * @param g [in]	The GPU driver struct.
 *
 * Calls function to do setup_sw. Refer #nvgpu_fifo_setup_sw.
 * If setup_sw was successful, call function to do setup_hw. This is to take
 * care of h/w specific setup related to FIFO module.
 *
 * @return 0 in case of success, < 0 in case of failure.
 * @retval Error returned by setup_sw and setup_hw routines.
 */
int nvgpu_fifo_init_support(struct gk20a *g);

/**
 * @brief Initialize FIFO software context and mark it ready to be used.
 *
 * @param g [in]	The GPU driver struct.
 *
 * Return if #nvgpu_fifo.sw_ready is set to true i.e. s/w set up is already
 * done.
 * Call #nvgpu_fifo_setup_sw_common to do s/w set up.
 * Init channel worker.
 * Mark FIFO s/w ready by setting #nvgpu_fifo.sw_ready to true.
 *
 * @return 0 in case of success, < 0 in case of failure.
 */
int nvgpu_fifo_setup_sw(struct gk20a *g);

/**
 * @brief Initialize FIFO software context.
 *
 * @param g [in]	The GPU driver struct.
 *
 * Initialize FIFO software context:
 * - Init mutexes needed by FIFO module. Refer #nvgpu_fifo struct.
 * - Do #nvgpu_channel_setup_sw.
 * - Do #nvgpu_tsg_setup_sw.
 * - Do pbdma.setup_sw.
 * - Do #nvgpu_engine_setup_sw.
 * - Do #nvgpu_runlist_setup_sw.
 * - Do userd.setup_sw.
 * - Init #nvgpu_fifo.remove_support function pointer.
 *
 * @return 0 in case of success, < 0 in case of failure.
 * @note In case of failure, cleanup_sw for the blocks that are already
 *       initialized is also taken care of by this function.
 */
int nvgpu_fifo_setup_sw_common(struct gk20a *g);

/**
 * @brief Clean up FIFO software context.
 *
 * @param g [in]	The GPU driver struct.
 *
 * Deinit Channel worker thread.
 * Calls #nvgpu_fifo_cleanup_sw_common.
 */
void nvgpu_fifo_cleanup_sw(struct gk20a *g);

/**
 * @brief Clean up FIFO software context and related resources.
 *
 * @param g [in]	The GPU driver struct.
 *
 * Clean up FIFO software context and related resources:
 * - Do userd.cleanup_sw.
 * - Do #nvgpu_channel_cleanup_sw.
 * - Do #nvgpu_tsg_cleanup_sw.
 * - Do #nvgpu_runlist_cleanup_sw.
 * - Do #nvgpu_engine_cleanup_sw.
 * - Do pbdma.setup_sw.
 * - Destroy mutexes used by FIFO module. Refer #nvgpu_fifo struct.
 */
void nvgpu_fifo_cleanup_sw_common(struct gk20a *g);

/**
 * @brief Decode PBDMA channel status and Engine status read from h/w register.
 *
 * @param index [in]	Status value used to index into the constant array of
 *			constant characters.
 *
 * Decode PBDMA channel status and Engine status value read from h/w
 * register into string format.
 */
const char *nvgpu_fifo_decode_pbdma_ch_eng_status(u32 index);

/**
 * @brief Suspend FIFO support while preparing GPU for poweroff.
 *
 * @param g [in]	The GPU driver struct.
 *
 * Suspending FIFO will disable BAR1 snooping (if supported by h/w) and also
 * FIFO interrupts.
 */
int nvgpu_fifo_suspend(struct gk20a *g);

#ifndef CONFIG_NVGPU_RECOVERY
/**
 * @brief Emergency quiescing of FIFO.
 *
 * @param g [in]	The GPU driver struct.
 *
 * Gracefully put FIFO into a non-functioning state to ensure that no corrupted
 * work is completed because of the fault. This is because the freedom
 * from interference may not always be shown between the faulted and
 * the non-faulted TSG contexts.
 * - Disable all runlists
 * - Preempt all runlists
 * - Quiesce all channels
 *
 * @see nvgpu_channel_sw_quiesce
 */
void nvgpu_fifo_sw_quiesce(struct gk20a *g);
#endif

#endif /* NVGPU_FIFO_COMMON_H */
