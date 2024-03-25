
#include <stages/adsp_pipeline.h>
#include <xcore/select.h>
#include <xcore/channel.h>
#include <xcore/hwtimer.h>
#include <xcore/thread.h>
#include <print.h>
#include "cmds.h" // Autogenerated
#include "cmd_offsets.h" // Autogenerated
#include <stages/bump_allocator.h>

// MAX macro
#define _ADSP_MAX(A, B) (((A) > (B)) ? (A) : (B))

#include <stages/pipeline.h>
#include <stages/dsp_thread.h>
#include <stages/cascaded_biquads.h>
#include <stages/volume_control.h>
#include <stages/dsp_thread.h>
#include <stages/biquad.h>
#include <stages/fork.h>
#include <pipeline_config.h>
#include <dsp_thread_config.h>
#include <cascaded_biquads_config.h>
#include <volume_control_config.h>
#include <dsp_thread_config.h>
#include <biquad_config.h>
DECLARE_JOB(dsp_auto_thread0, (chanend_t*, chanend_t*, module_instance_t**));
void dsp_auto_thread0(chanend_t* c_source, chanend_t* c_dest, module_instance_t** modules) {
	local_thread_mode_set_bits(thread_mode_high_priority);	int32_t edge0[1] = {0};
	int32_t edge1[1] = {0};
	int32_t edge2[1] = {0};
	int32_t edge3[1] = {0};
	int32_t edge4[1] = {0};
	int32_t edge5[1] = {0};
	int32_t** stage_0_output = NULL;
	(void)stage_0_output;	int32_t** stage_1_output = NULL;
	(void)stage_1_output;	int32_t* stage_2_input[] = {edge0, edge1};
	int32_t* stage_2_output[] = {edge2, edge3};
	int32_t* stage_3_input[] = {edge2, edge3};
	int32_t* stage_3_output[] = {edge4, edge5};
	uint32_t start_ts, end_ts, start_control_ts, control_ticks;
	bool control_done;
	while(1) {
	control_done = false;
	int read_count = 1;
	SELECT_RES(
		CASE_THEN(c_source[0], case_0),
		DEFAULT_THEN(do_control)
	) {
		case_0: {
			for(int idx = 0; idx < 1; ++idx) edge0[idx] = ((int32_t)chan_in_word(c_source[0])) >> 4;
			for(int idx = 0; idx < 1; ++idx) edge1[idx] = ((int32_t)chan_in_word(c_source[0])) >> 4;
			if(!--read_count) break;
			else continue;
		}
		do_control: {
		start_control_ts = get_reference_time();
		pipeline_control(modules[0]->state, &modules[0]->control);
		dsp_thread_control(modules[1]->state, &modules[1]->control);
		cascaded_biquads_control(modules[2]->state, &modules[2]->control);
		volume_control_control(modules[3]->state, &modules[3]->control);
		control_done = true;
		control_ticks = get_reference_time() - start_control_ts;
		continue; }
	}
	if(!control_done){
		start_control_ts = get_reference_time();
		pipeline_control(modules[0]->state, &modules[0]->control);
		dsp_thread_control(modules[1]->state, &modules[1]->control);
		cascaded_biquads_control(modules[2]->state, &modules[2]->control);
		volume_control_control(modules[3]->state, &modules[3]->control);
		control_ticks = get_reference_time() - start_control_ts;
	}
	start_ts = get_reference_time();

	cascaded_biquads_process(
		stage_2_input,
		stage_2_output,
		modules[2]->state);
	volume_control_process(
		stage_3_input,
		stage_3_output,
		modules[3]->state);

	end_ts = get_reference_time();
	uint32_t process_plus_control_ticks = (end_ts - start_ts) + control_ticks;
	if(process_plus_control_ticks > ((dsp_thread_state_t*)(modules[1]->state))->max_cycles)
	{
		((dsp_thread_state_t*)(modules[1]->state))->max_cycles = process_plus_control_ticks;
	}
	chan_out_buf_word(c_dest[0], (void*)edge4, 1);
	chan_out_buf_word(c_dest[0], (void*)edge5, 1);
	}
}
DECLARE_JOB(dsp_auto_thread1, (chanend_t*, chanend_t*, module_instance_t**));
void dsp_auto_thread1(chanend_t* c_source, chanend_t* c_dest, module_instance_t** modules) {
	local_thread_mode_set_bits(thread_mode_high_priority);	int32_t edge0[1] = {0};
	int32_t edge1[1] = {0};
	int32_t edge2[1] = {0};
	int32_t edge3[1] = {0};
	int32_t edge4[1] = {0};
	int32_t edge5[1] = {0};
	int32_t edge6[1] = {0};
	int32_t edge7[1] = {0};
	int32_t** stage_0_output = NULL;
	(void)stage_0_output;	int32_t* stage_1_input[] = {edge0, edge1};
	int32_t* stage_1_output[] = {edge2, edge3};
	int32_t* stage_2_input[] = {edge2, edge3};
	int32_t* stage_2_output[] = {edge4, edge5, edge6, edge7};
	uint32_t start_ts, end_ts, start_control_ts, control_ticks;
	bool control_done;
	while(1) {
	control_done = false;
	for(int idx = 0; idx < 1; ++idx) chan_out_word(c_dest[0], (edge4[idx] < -134217728) ? -2147483648 : ((edge4[idx] > 134217727) ? 2147483647 : (edge4[idx] << 4)));
	for(int idx = 0; idx < 1; ++idx) chan_out_word(c_dest[0], (edge5[idx] < -134217728) ? -2147483648 : ((edge5[idx] > 134217727) ? 2147483647 : (edge5[idx] << 4)));
	for(int idx = 0; idx < 1; ++idx) chan_out_word(c_dest[0], (edge6[idx] < -134217728) ? -2147483648 : ((edge6[idx] > 134217727) ? 2147483647 : (edge6[idx] << 4)));
	for(int idx = 0; idx < 1; ++idx) chan_out_word(c_dest[0], (edge7[idx] < -134217728) ? -2147483648 : ((edge7[idx] > 134217727) ? 2147483647 : (edge7[idx] << 4)));
	int read_count = 1;
	SELECT_RES(
		CASE_THEN(c_source[0], case_0),
		DEFAULT_THEN(do_control)
	) {
		case_0: {
			chan_in_buf_word(c_source[0], (void*)edge0, 1);
			chan_in_buf_word(c_source[0], (void*)edge1, 1);
			if(!--read_count) break;
			else continue;
		}
		do_control: {
		start_control_ts = get_reference_time();
		dsp_thread_control(modules[0]->state, &modules[0]->control);
		biquad_control(modules[1]->state, &modules[1]->control);
		control_done = true;
		control_ticks = get_reference_time() - start_control_ts;
		continue; }
	}
	if(!control_done){
		start_control_ts = get_reference_time();
		dsp_thread_control(modules[0]->state, &modules[0]->control);
		biquad_control(modules[1]->state, &modules[1]->control);
		control_ticks = get_reference_time() - start_control_ts;
	}
	start_ts = get_reference_time();

	biquad_process(
		stage_1_input,
		stage_1_output,
		modules[1]->state);
	fork_process(
		stage_2_input,
		stage_2_output,
		modules[2]->state);

	end_ts = get_reference_time();
	uint32_t process_plus_control_ticks = (end_ts - start_ts) + control_ticks;
	if(process_plus_control_ticks > ((dsp_thread_state_t*)(modules[0]->state))->max_cycles)
	{
		((dsp_thread_state_t*)(modules[0]->state))->max_cycles = process_plus_control_ticks;
	}
	}
}
adsp_pipeline_t * adsp_auto_pipeline_init() {
	static adsp_pipeline_t adsp_auto;

	static channel_t adsp_auto_in_chans[1];
	static channel_t adsp_auto_out_chans[1];
	static channel_t adsp_auto_link_chans[1];
	static module_instance_t adsp_auto_modules[7];
	static adsp_mux_elem_t adsp_auto_in_mux_cfgs[] = {
		{ .channel_idx = 0, .data_idx = 0, .frame_size = 1},
		{ .channel_idx = 0, .data_idx = 1, .frame_size = 1},
	};
	static adsp_mux_elem_t adsp_auto_out_mux_cfgs[] = {
		{ .channel_idx = 0, .data_idx = 0, .frame_size = 1},
		{ .channel_idx = 0, .data_idx = 2, .frame_size = 1},
		{ .channel_idx = 0, .data_idx = 1, .frame_size = 1},
		{ .channel_idx = 0, .data_idx = 3, .frame_size = 1},
	};
	adsp_auto.input_mux.n_chan = 2;
	adsp_auto.input_mux.chan_cfg = (adsp_mux_elem_t *) adsp_auto_in_mux_cfgs;
	adsp_auto.output_mux.n_chan = 4;
	adsp_auto.output_mux.chan_cfg = (adsp_mux_elem_t *) adsp_auto_out_mux_cfgs;
	adsp_auto_in_chans[0] = chan_alloc();
	adsp_auto_out_chans[0] = chan_alloc();
	adsp_auto_link_chans[0] = chan_alloc();
	adsp_auto.p_in = (channel_t *) adsp_auto_in_chans;
	adsp_auto.n_in = 1;
	adsp_auto.p_out = (channel_t *) adsp_auto_out_chans;
	adsp_auto.n_out = 1;
	adsp_auto.p_link = (channel_t *) adsp_auto_link_chans;
	adsp_auto.n_link = 1;
	adsp_auto.modules = adsp_auto_modules;
	adsp_auto.n_modules = 7;
	static pipeline_config_t config0 = { .checksum = {191, 230, 139, 212, 99, 188, 211, 90, 97, 211, 183, 25, 198, 42, 200, 217} };

            static pipeline_state_t state0;
            static uint8_t memory0[_ADSP_MAX(1, PIPELINE_REQUIRED_MEMORY(0, 0, 1))];
            static adsp_bump_allocator_t allocator0 = ADSP_BUMP_ALLOCATOR_INITIALISER(memory0);

            adsp_auto.modules[0].state = (void*)&state0;

            // Control stuff
            adsp_auto.modules[0].control.id = 0;
            adsp_auto.modules[0].control.config_rw_state = config_none_pending;
            
                adsp_auto.modules[0].control.config = (void*)&config0;
                adsp_auto.modules[0].control.module_type = e_dsp_stage_pipeline;
                adsp_auto.modules[0].control.num_control_commands = NUM_CMDS_PIPELINE;
                pipeline_init(&adsp_auto.modules[0], &allocator0, 0, 0, 0, 1);
	static dsp_thread_config_t config1 = {  };

            static dsp_thread_state_t state1;
            static uint8_t memory1[_ADSP_MAX(1, DSP_THREAD_REQUIRED_MEMORY(0, 0, 1))];
            static adsp_bump_allocator_t allocator1 = ADSP_BUMP_ALLOCATOR_INITIALISER(memory1);

            adsp_auto.modules[1].state = (void*)&state1;

            // Control stuff
            adsp_auto.modules[1].control.id = 1;
            adsp_auto.modules[1].control.config_rw_state = config_none_pending;
            
                adsp_auto.modules[1].control.config = (void*)&config1;
                adsp_auto.modules[1].control.module_type = e_dsp_stage_dsp_thread;
                adsp_auto.modules[1].control.num_control_commands = NUM_CMDS_DSP_THREAD;
                dsp_thread_init(&adsp_auto.modules[1], &allocator1, 1, 0, 0, 1);
	static cascaded_biquads_config_t config2 = { .left_shift = {0, 0, 0, 0, 0, 0, 0, 0}, .filter_coeffs = {839528639, -1679057279, 839528639, 1649955220, -634417514, 848230047, -1696460094, 848230047, 1667056403, -652121961, 865725974, -1731451947, 865725974, 1701441764, -687720307, 892169957, -1784339914, 892169957, 1753413056, -741524947, 927703577, -1855407154, 927703577, 1823248532, -813823952, 972331088, -1944662176, 972331088, 1910956552, -904625976, 1025724397, -2051448794, 1025724397, 2015892304, -1013263460, 1073741824, 0, 0, 0, 0} };

            static cascaded_biquads_state_t state2;
            static uint8_t memory2[_ADSP_MAX(1, CASCADED_BIQUADS_REQUIRED_MEMORY(2, 2, 1))];
            static adsp_bump_allocator_t allocator2 = ADSP_BUMP_ALLOCATOR_INITIALISER(memory2);

            adsp_auto.modules[2].state = (void*)&state2;

            // Control stuff
            adsp_auto.modules[2].control.id = 2;
            adsp_auto.modules[2].control.config_rw_state = config_none_pending;
            
                adsp_auto.modules[2].control.config = (void*)&config2;
                adsp_auto.modules[2].control.module_type = e_dsp_stage_cascaded_biquads;
                adsp_auto.modules[2].control.num_control_commands = NUM_CMDS_CASCADED_BIQUADS;
                cascaded_biquads_init(&adsp_auto.modules[2], &allocator2, 2, 2, 2, 1);
	static volume_control_config_t config3 = { .target_gain = 134217728, .slew_shift = 2, .mute = 0 };

            static volume_control_state_t state3;
            static uint8_t memory3[_ADSP_MAX(1, VOLUME_CONTROL_REQUIRED_MEMORY(2, 2, 1))];
            static adsp_bump_allocator_t allocator3 = ADSP_BUMP_ALLOCATOR_INITIALISER(memory3);

            adsp_auto.modules[3].state = (void*)&state3;

            // Control stuff
            adsp_auto.modules[3].control.id = 3;
            adsp_auto.modules[3].control.config_rw_state = config_none_pending;
            
                adsp_auto.modules[3].control.config = (void*)&config3;
                adsp_auto.modules[3].control.module_type = e_dsp_stage_volume_control;
                adsp_auto.modules[3].control.num_control_commands = NUM_CMDS_VOLUME_CONTROL;
                volume_control_init(&adsp_auto.modules[3], &allocator3, 3, 2, 2, 1);
	static dsp_thread_config_t config4 = {  };

            static dsp_thread_state_t state4;
            static uint8_t memory4[_ADSP_MAX(1, DSP_THREAD_REQUIRED_MEMORY(0, 0, 1))];
            static adsp_bump_allocator_t allocator4 = ADSP_BUMP_ALLOCATOR_INITIALISER(memory4);

            adsp_auto.modules[4].state = (void*)&state4;

            // Control stuff
            adsp_auto.modules[4].control.id = 4;
            adsp_auto.modules[4].control.config_rw_state = config_none_pending;
            
                adsp_auto.modules[4].control.config = (void*)&config4;
                adsp_auto.modules[4].control.module_type = e_dsp_stage_dsp_thread;
                adsp_auto.modules[4].control.num_control_commands = NUM_CMDS_DSP_THREAD;
                dsp_thread_init(&adsp_auto.modules[4], &allocator4, 4, 0, 0, 1);
	static biquad_config_t config5 = { .left_shift = 0, .filter_coeffs = {1022611261, -1771214660, 1022611261, 1771214660, -971480698} };

            static biquad_state_t state5;
            static uint8_t memory5[_ADSP_MAX(1, BIQUAD_REQUIRED_MEMORY(2, 2, 1))];
            static adsp_bump_allocator_t allocator5 = ADSP_BUMP_ALLOCATOR_INITIALISER(memory5);

            adsp_auto.modules[5].state = (void*)&state5;

            // Control stuff
            adsp_auto.modules[5].control.id = 5;
            adsp_auto.modules[5].control.config_rw_state = config_none_pending;
            
                adsp_auto.modules[5].control.config = (void*)&config5;
                adsp_auto.modules[5].control.module_type = e_dsp_stage_biquad;
                adsp_auto.modules[5].control.num_control_commands = NUM_CMDS_BIQUAD;
                biquad_init(&adsp_auto.modules[5], &allocator5, 5, 2, 2, 1);

            static fork_state_t state6;
            static uint8_t memory6[_ADSP_MAX(1, FORK_REQUIRED_MEMORY(2, 4, 1))];
            static adsp_bump_allocator_t allocator6 = ADSP_BUMP_ALLOCATOR_INITIALISER(memory6);

            adsp_auto.modules[6].state = (void*)&state6;

            // Control stuff
            adsp_auto.modules[6].control.id = 6;
            adsp_auto.modules[6].control.config_rw_state = config_none_pending;
            
                adsp_auto.modules[6].control.config = NULL;
                adsp_auto.modules[6].control.num_control_commands = 0;
                fork_init(&adsp_auto.modules[6], &allocator6, 6, 2, 4, 1);
	return &adsp_auto;
}

void adsp_auto_pipeline_main(adsp_pipeline_t* adsp) {
	module_instance_t* thread_0_modules[] = {
		&adsp->modules[0],
		&adsp->modules[1],
		&adsp->modules[2],
		&adsp->modules[3],
	};
	chanend_t thread_0_inputs[] = {
		adsp->p_in[0].end_b};
	chanend_t thread_0_outputs[] = {
		adsp->p_link[0].end_a};
	module_instance_t* thread_1_modules[] = {
		&adsp->modules[4],
		&adsp->modules[5],
		&adsp->modules[6],
	};
	chanend_t thread_1_inputs[] = {
		adsp->p_link[0].end_b};
	chanend_t thread_1_outputs[] = {
		adsp->p_out[0].end_a};
	PAR_JOBS(
		PJOB(dsp_auto_thread0, (thread_0_inputs, thread_0_outputs, thread_0_modules)),
		PJOB(dsp_auto_thread1, (thread_1_inputs, thread_1_outputs, thread_1_modules))
	);
}