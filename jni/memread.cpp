#include <jni.h>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

// Helper to scrape /proc/self/maps to grab the game base memory mapping address
uintptr_t get_sandbox_module_base(const char* target_library) {
    uintptr_t base_addr = 0;
    std::ifstream maps("/proc/self/maps");
    std::string line;

    while (std::getline(maps, line)) {
        if (line.find(target_library) != std::string::npos) {
            base_addr = std::stoull(line.substr(0, line.find("-")), nullptr, 16);
            break; // Grab the primary text segment layout base pointer
        }
    }
    return base_addr;
}

extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_predictor_OverlayService_getLiveGameVectors(JNIEnv* env, jobject thiz) {
    // Look for the loaded Carrom engine split module footprint inside the virtual container footprint
    uintptr_t game_base = get_sandbox_module_base("libgame-CARROM");
    
    if (game_base == 0) {
        return nullptr; // Game structure not initialized or accessible in memory stack yet
    }

    // Known static pointer offsets extracted from your Ghidra MCP analysis report
    uintptr_t table_logic_offset = 0x02f7a190; 
    uintptr_t table_logic_ptr = *reinterpret_cast<uintptr_t*>(game_base + table_logic_offset);
    
    if (table_logic_ptr == 0) {
        return nullptr;
    }

    // Track primary Striker node references via TableLogic collection array offset
    uintptr_t striker_entity_ptr = *reinterpret_cast<uintptr_t*>(table_logic_ptr + 0x168);
    if (striker_entity_ptr == 0) {
        return nullptr;
    }

    // Extract core float/double variables straight out of the verified entity matrix
    double pos_x = *reinterpret_cast<double*>(striker_entity_ptr + 0x10);
    double pos_y = *reinterpret_cast<double*>(striker_entity_ptr + 0x18);
    double vel_x = *reinterpret_cast<double*>(striker_entity_ptr + 0x50);
    double vel_y = *reinterpret_cast<double*>(striker_entity_ptr + 0x58);

    // Apply the game's exact documented Euler integration loop (pos += vel * dt) 
    // to map future trajectory vectors down to 60 FPS sub-steps locally
    float time_step = 0.016f; 
    float predicted_x = static_cast<float>(pos_x + (vel_x * time_step));
    float predicted_y = static_cast<float>(pos_y + (vel_y * time_step));

    // Package output vector arrays to feed straight into Java's drawing canvas
    jfloatArray result_array = env->NewFloatArray(4);
    if (result_array == nullptr) return nullptr;

    jfloat output_buffers[4];
    output_buffers[0] = static_cast<float>(pos_x);
    output_buffers[1] = static_cast<float>(pos_y);
    output_buffers[2] = predicted_x;
    output_buffers[3] = predicted_y;

    env->SetFloatArrayRegion(result_array, 0, 4, output_buffers);
    return result_array;
}
