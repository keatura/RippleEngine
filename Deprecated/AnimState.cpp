//#include "AnimState.h"
//
//void AnimState::PlayOverride(uint32_t state_hash, int64_t curr_frame, int duration)
//{
//	curr_override_hash = state_hash;
//	override_start_frame = curr_frame;
//	override_duration = duration;
//}
//
//bool AnimState::IsInOverride(int64_t curr_frame) const {
//	return curr_override_hash != 0 && (curr_frame - override_start_frame < override_duration);
//}
//
//uint32_t AnimState::GetTextureIdx(uint32_t hash) const {
//	for (uint8_t i = 0; i < num_clips; i++) {
//		if (clips[i].anim_state_hash == hash) return clips[i].texture_idx;
//	}
//	return UINT32_MAX;
//}