//#pragma once
//
//#include <string>
//#include <cstdint>
//struct alignas(sizeof(uint32_t) * 2) AnimClip {
//	uint32_t anim_state_hash;
//	uint32_t texture_idx;
//};
//
//class AnimState
//{
//public:
//	AnimClip clips[8]; // if you need more no you dont
//	uint8_t num_clips = 0;
//
//	uint32_t curr_override_hash = 0;
//	uint32_t base_state_hash = 0; // same as saying "main"
//
//	int64_t override_start_frame = -100;
//	int override_duration = 30;
//
//	void PlayOverride(uint32_t state_hash, int64_t curr_frame, int duration = 30);
//	bool IsInOverride(int64_t curr_frame) const;
//	uint32_t GetTextureIdx(uint32_t hash) const;
//};
//
