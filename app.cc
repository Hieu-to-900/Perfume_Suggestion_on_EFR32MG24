/***************************************************************************//**
 * @file app.cc
 * @brief application functions.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "app.h"
#include "timestamp.h"
#include "sl_pwm_instances.h"
#include "sl_pwm.h"
#include "sl_sleeptimer.h"
#include "sl_tflite_micro_init.h"
#include <stdio.h>

//#include <cstdio>
//#include <cstdint>
#include <string>
#include <vector>
//#include <unordered_map>
#include <cmath>

#include "perfume_data.h"

void generate_all_perfume_embeddings();

static tflite::MicroInterpreter *interpreter;
//static TfLiteTensor *input;
//static TfLiteTensor *output;
float all_perfume_embeddings[50][64];

// Định nghĩa kích thước đầu vào và đầu ra của mô hình TFLite
#define NUM_GENDERS 3   // men, women, unisex
#define NUM_BRANDS 50   // Giả sử có 50 thương hiệu, giá trị này không đổi
#define NUM_NOTES 193
#define NUM_ACCORDS 42
#define NUM_NUMERICAL_FEATURES 5 // Dựa trên 5 features số đã định nghĩa

void multi_hot_encode_for_notes(const char* const* items,
                            uint8_t items_count,
                            const NoteIndex* vocab_to_idx,
                            size_t vocab_size,
                            int32_t* output_array,
                            size_t output_size) {
    // 1. Khởi tạo mảng đầu ra về 0
    for (size_t i = 0; i < output_size; ++i) {
        output_array[i] = 0;
    }

    // 2. Duyệt qua từng item đầu vào
    for (uint8_t i = 0; i < items_count; ++i) {
        // 3. So sánh item với từng chuỗi trong từ vựng (linear search)
        for (size_t j = 0; j < vocab_size; ++j) {
            // Sử dụng strcmp để so sánh chuỗi
            if (strcmp(items[i], vocab_to_idx[j].name) == 0) {
                // Nếu tìm thấy, đánh dấu 1 vào vị trí tương ứng trong mảng đầu ra
                output_array[vocab_to_idx[j].index] = 1;
                // Thoát vòng lặp trong để tối ưu hóa
                break;
            }
        }
    }
}

void multi_hot_encode_for_accord(const char* const* items,
                            uint8_t items_count,
                            const AccordEntry* vocab_to_idx,
                            size_t vocab_size,
                            int32_t* output_array,
                            size_t output_size) {
    // 1. Khởi tạo mảng đầu ra về 0
    for (size_t i = 0; i < output_size; ++i) {
        output_array[i] = 0;
    }

    // 2. Duyệt qua từng item đầu vào
    for (uint8_t i = 0; i < items_count; ++i) {
        // 3. So sánh item với từng chuỗi trong từ vựng (linear search)
        for (size_t j = 0; j < vocab_size; ++j) {
            // Sử dụng strcmp để so sánh chuỗi
            if (strcmp(items[i], vocab_to_idx[j].name) == 0) {
                // Nếu tìm thấy, đánh dấu 1 vào vị trí tương ứng trong mảng đầu ra
                output_array[vocab_to_idx[j].index] = 1;
                // Thoát vòng lặp trong để tối ưu hóa
                break;
            }
        }
    }
}

// Hàm so sánh cho việc sắp xếp
int compareSimilarity(const void * a, const void * b) {
    float sim_a = ((SimilarityResult*)a)->similarity;
    float sim_b = ((SimilarityResult*)b)->similarity;
    if (sim_a > sim_b) return -1;
    if (sim_a < sim_b) return 1;
    return 0;
}

// Hàm tính toán độ tương đồng cosine
float cosine_similarity(const float* vec1, const float* vec2, size_t size) {
    float dot_product = 0.0f;
    float magnitude1 = 0.0f;
    float magnitude2 = 0.0f;

    for (size_t i = 0; i < size; ++i) {
        dot_product += vec1[i] * vec2[i];
        magnitude1 += vec1[i] * vec1[i];
        magnitude2 += vec2[i] * vec2[i];
    }

    if (magnitude1 == 0.0f || magnitude2 == 0.0f) {
        return 0.0f;
    }

    return dot_product / (sqrtf(magnitude1) * sqrtf(magnitude2));
}

void DumpAllInputs(tflite::MicroInterpreter* interp) {
  int n = interp->inputs_size(); // nếu không có dùng alternatif
  // fallback: thử input(i) cho i = 0..9
  for (int i = 0; i < 12; ++i) {
    TfLiteTensor* t = interp->input(i);
    if (!t) continue;
    printf("input[%d]: type=%d, dims=[", i, t->type);
    for (int d = 0; d < t->dims->size; ++d) {
      printf("%d%s", t->dims->data[d], (d+1<t->dims->size)?",":"");
    }
    printf("], bytes=%d, data=%p\n", t->bytes, t->data.raw);
  }
}

void app_init(void)
{
  printf("Start App init done!\n");

  interpreter = sl_tflite_micro_get_interpreter();
//  input = sl_tflite_micro_get_input_tensor();
//  output = sl_tflite_micro_get_output_tensor();
  DumpAllInputs(interpreter);

  generate_all_perfume_embeddings();
}

static void DumpTensor(const TfLiteTensor* t, const char* name) {
  printf("%s: type=%d, dims=[", name, t->type);
  for (int i = 0; i < t->dims->size; ++i) printf("%d%s", t->dims->data[i], i+1<t->dims->size?",":"");
  printf("], bytes=%d, data=%p\n", t->bytes, t->data.raw);
}

void debug_check_gather(tflite::MicroInterpreter* interp) {
  // Thay các chỉ số dưới cho đúng với model của bạn:
  // - input(0): data tensor mà Gather sẽ lấy phần tử từ đó
  // - input(1): indices tensor
  // - output(0): output của Gather
  const TfLiteTensor* in0 = interp->input_tensor(0);
  const TfLiteTensor* in1 = interp->input_tensor(1);
  const TfLiteTensor* out0 = interp->output_tensor(0); // nếu Gather là op đầu/đơn giản

  DumpTensor(in0, "Gather.in0(data)");
  DumpTensor(in1, "Gather.in1(indices)");
  DumpTensor(out0, "Gather.out0");

  // In vài giá trị index đầu (nếu int32):
  if (in1->type == kTfLiteInt32) {
    auto idx = reinterpret_cast<const int32_t*>(in1->data.raw);
    int n = in1->bytes / sizeof(int32_t);
    printf("indices count=%d, first up to 16: ", n);
    for (int i = 0; i < n && i < 16; ++i) printf("%d ", idx[i]);
    printf("\n");
  }
}

void generate_all_perfume_embeddings() {
  printf("Generate_all_perfume_embeddings!\n");

  // Tìm và map các input theo dims (an toàn hơn là giả định index cố định)
  TfLiteTensor* in0 = interpreter->input(0);
  TfLiteTensor* in1 = interpreter->input(1);
  TfLiteTensor* in2 = interpreter->input(2);
  TfLiteTensor* in3 = interpreter->input(3);
  TfLiteTensor* in4 = interpreter->input(4);
  TfLiteTensor* in5 = interpreter->input(5);

  // Kiểm tra sơ bộ
  if (!in0 || !in1 || !in2 || !in3 || !in4 || !in5) {
    printf("Error: one of input tensors is null\n");
    return;
  }

  // Xác nhận mapping: theo dump của bạn
  // notes -> input[0] (dims [1,193], bytes 772 -> int32)
  // accords -> input[1] (dims [1,42], bytes 168 -> int32)
  // numerical -> input[2] (dims [1,5], bytes 5)
  // gender -> input[3] (1 element)
  // brand  -> input[4] (1 element)
  // year   -> input[5] (1 element, bytes=1)
  TfLiteTensor *input_notes = in0;
  TfLiteTensor *input_accords = in1;
  TfLiteTensor *input_numerical = in2;
  TfLiteTensor *input_gender = in3;
  TfLiteTensor *input_brand = in4;
  TfLiteTensor *input_year = in5;

  // tạm buffers
  static int32_t notes_encoded[NUM_NOTES];
  static int32_t accords_encoded[NUM_ACCORDS];
  static float numerical_features[NUM_NUMERICAL_FEATURES];

  printf("Mapping: notes@%p bytes=%d, accords@%p bytes=%d, numerical@%p bytes=%d\n",
         input_notes->data.raw, input_notes->bytes,
         input_accords->data.raw, input_accords->bytes,
         input_numerical->data.raw, input_numerical->bytes);
  printf("single inputs: gender@%p bytes=%d, brand@%p bytes=%d, year@%p bytes=%d\n",
         input_gender->data.raw, input_gender->bytes,
         input_brand->data.raw, input_brand->bytes,
         input_year->data.raw, input_year->bytes);

  // sanity: đảm bảo notes_slots >= NUM_NOTES (ở model của bạn notes_slots == 193)
  int notes_slots_bytes = input_notes->bytes;
  int accords_slots_bytes = input_accords->bytes;

  if (notes_slots_bytes < NUM_NOTES * (int)sizeof(int32_t)) {
    printf("FATAL: model input notes has only %d bytes (< %d needed). Aborting.\n",
           notes_slots_bytes, NUM_NOTES * (int)sizeof(int32_t));
    return;
  }
  if (accords_slots_bytes < NUM_ACCORDS * (int)sizeof(int32_t)) {
    printf("FATAL: model input accords has only %d bytes (< %d needed). Aborting.\n",
           accords_slots_bytes, NUM_ACCORDS * (int)sizeof(int32_t));
    return;
  }

  for (int i = 0; i < 50; ++i) {
    // reset tmp buffers
    memset(notes_encoded, 0, sizeof(notes_encoded));
    memset(accords_encoded, 0, sizeof(accords_encoded));
    memset(numerical_features, 0, sizeof(numerical_features));

    const PerfumeRO &data = all_perfume_data[i];
    int32_t gender_code = data.gender;    // nếu bạn đã mã hoá gender thành số
    int32_t brand_code  = 0.0f;     // nếu có
    int32_t year_code   = 1.0f;      // nếu có

    // multi-hot encode (hãy đảm bảo các hàm này respect output_size)
    multi_hot_encode_for_notes(data.notes, data.notes_count, notes_to_idx, NUM_NOTES, notes_encoded, NUM_NOTES);
    multi_hot_encode_for_accord(data.accords, data.accords_count, accords_to_idx, NUM_ACCORDS, accords_encoded, NUM_ACCORDS);

    numerical_features[0] = data.weighted_rating;
    numerical_features[1] = 100.0f;
    numerical_features[2] = data.weighted_rating;
    numerical_features[3] = static_cast<float>(data.longevity);
    numerical_features[4] = static_cast<float>(data.sillage);

    // --- copy notes (model expects int32 per dump) ---
    if (input_notes->type == kTfLiteInt32) {
      int32_t* dst = reinterpret_cast<int32_t*>(input_notes->data.raw);
      int dst_count = input_notes->bytes / sizeof(int32_t); // should be 193
      int copy_count = (NUM_NOTES < dst_count) ? NUM_NOTES : dst_count;
      for (int j = 0; j < copy_count; ++j) {
        dst[j] = notes_encoded[j];
      }
    } else if (input_notes->type == kTfLiteInt8 || input_notes->type == kTfLiteUInt8) {
      // nếu model dùng int8, phải chuyển scale/zero-point (không quên clamp)
      int8_t* dst = reinterpret_cast<int8_t*>(input_notes->data.raw);
      int dst_count = input_notes->bytes / sizeof(int8_t);
      int copy_count = (NUM_NOTES < dst_count) ? NUM_NOTES : dst_count;
      for (int j = 0; j < copy_count; ++j) {
        int v = notes_encoded[j];
        if (v < -128) v = -128;
        if (v > 127)  v = 127;
        dst[j] = static_cast<int8_t>(v);
      }
    } else {
      printf("Unsupported notes tensor type=%d\n", input_notes->type);
      return;
    }

    // --- copy accords (int32 per dump) ---
    if (input_accords->type == kTfLiteInt32) {
      int32_t* dst = reinterpret_cast<int32_t*>(input_accords->data.raw);
      int dst_count = input_accords->bytes / sizeof(int32_t); // 42
      int copy_count = (NUM_ACCORDS < dst_count) ? NUM_ACCORDS : dst_count;
      for (int j = 0; j < copy_count; ++j) dst[j] = accords_encoded[j];
    } else {
      // handle int8 case similarly if needed
      printf("Warning: input_accords type=%d not int32\n", input_accords->type);
    }

    // --- copy numerical features ---
    // numerical input in your dump had bytes=5; check type
    if (input_numerical->bytes < NUM_NUMERICAL_FEATURES) {
      // Might be quantized uint8 with one byte per feature -> need quantization
      // Here we attempt to write up to its capacity safely:
      int slot_count = input_numerical->bytes; // each slot is 1 byte in your dump
      // do quantization into uint8/int8 depending on type
      if (input_numerical->type == kTfLiteUInt8 || input_numerical->type == kTfLiteInt8) {
        uint8_t* dst = reinterpret_cast<uint8_t*>(input_numerical->data.raw);
        for (int j = 0; j < slot_count && j < NUM_NUMERICAL_FEATURES; ++j) {
          // quantize: use params.scale/zero_point
          float real = numerical_features[j];
          int32_t q = static_cast<int32_t>(round(real / input_numerical->params.scale + input_numerical->params.zero_point));
          if (q < 0) q = 0;
          if (q > 255) q = 255;
          dst[j] = static_cast<uint8_t>(q);
        }
      } else {
        printf("Unsupported numerical input type=%d bytes=%d\n", input_numerical->type, input_numerical->bytes);
      }
    } else {
      // enough capacity (unlikely in your dump), handle int8/int32 similarly
      if (input_numerical->type == kTfLiteInt32) {
        int32_t* dst = reinterpret_cast<int32_t*>(input_numerical->data.raw);
        int copy_count = (NUM_NUMERICAL_FEATURES < (input_numerical->bytes / 4)) ? NUM_NUMERICAL_FEATURES : (input_numerical->bytes / 4);
        for (int j = 0; j < copy_count; ++j) dst[j] = static_cast<int32_t>(numerical_features[j]);
      } else {
        // quantize as above if int8/uint8
      }
    }

    // --- copy single-value inputs (gender/brand/year) ---
    // They are 1-element int32 according to dump
    if (input_gender->bytes >= 4 && input_gender->type == kTfLiteInt32) {
      reinterpret_cast<int32_t*>(input_gender->data.raw)[0] = gender_code;
    } else {
      // if 1-byte quantized, quantize value
      if (input_gender->bytes == 1) {
        reinterpret_cast<int8_t*>(input_gender->data.raw)[0] =
            static_cast<int8_t>(round(gender_code / input_gender->params.scale + input_gender->params.zero_point));
      }
    }
    // brand
    if (input_brand->bytes >= 4 && input_brand->type == kTfLiteInt32) {
      reinterpret_cast<int32_t*>(input_brand->data.raw)[0] = brand_code;
    } else if (input_brand->bytes == 1) {
      reinterpret_cast<int8_t*>(input_brand->data.raw)[0] =
          static_cast<int8_t>(round(brand_code / input_brand->params.scale + input_brand->params.zero_point));
    }
    // year
    if (input_year->bytes >= 4 && input_year->type == kTfLiteInt32) {
      reinterpret_cast<int32_t*>(input_year->data.raw)[0] = year_code;
    } else if (input_year->bytes == 1) {
      reinterpret_cast<int8_t*>(input_year->data.raw)[0] =
          static_cast<int8_t>(round(year_code / input_year->params.scale + input_year->params.zero_point));
    }

    // Debug + Validate indices before invoke (you have debug_check_gather)
//    printf("ArenaUsed=%d\n", interpreter->arena_used_bytes());
//    debug_check_gather(interpreter);

    // Invoke
    TfLiteStatus status = interpreter->Invoke();
    if (status != kTfLiteOk) {
      printf("Inference failed at i=%d\n", i);
      continue;
    }

    // Read output safely
    TfLiteTensor* output = interpreter->output(0);
    int out_bytes = output->bytes;
    int out_count = out_bytes / sizeof(int8_t); // expect 64
    int max_store = sizeof(all_perfume_embeddings[0]) / sizeof(all_perfume_embeddings[0][0]);
    int store_count = (out_count < max_store) ? out_count : max_store;
    int8_t* out_data = output->data.int8;
    for (int j = 0; j < store_count; ++j) {
      all_perfume_embeddings[i][j] = (static_cast<int32_t>(out_data[j]) - output->params.zero_point) * output->params.scale;
    }
  } // for i

  printf("Done above!\n");
}

static const PerfumeRO test_user =   {
    /* gender    */ G_WOMEN,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 2,
    /* acc_cnt   */ 2,
    /* rating    */ 4.5f,
    /* notes     */ { "citruses", "lemon" },
    /* accords   */ { "white floral", "sweet" }
};


// Hàm xử lý đầu ra và in kết quả gợi ý
void handle_output(TfLiteTensor* output) {
    // 1. Giải lượng tử hóa vector nhúng của người dùng
    int8_t* output_data = output->data.int8;
    float output_scale = output->params.scale;
    int32_t output_zero_point = output->params.zero_point;
    size_t embedding_size = output->dims->data[1];

    float user_embedding[embedding_size];
    for (size_t j = 0; j < embedding_size; ++j) {
        user_embedding[j] = (output_data[j] - output_zero_point) * output_scale;
    }

    // 2. Tính toán độ tương đồng với tất cả 50 loại nước hoa
    SimilarityResult results[50];
    for (int i = 0; i < 50; ++i) {
        results[i].index = i;
        results[i].similarity = cosine_similarity(user_embedding, all_perfume_embeddings[i], embedding_size);
    }

    // 3. Sắp xếp kết quả theo độ tương đồng giảm dần
    qsort(results, 50, sizeof(SimilarityResult), compareSimilarity);

    // 4. In ra 10 kết quả có độ tương đồng cao nhất
    printf("Top 10 recommended perfumes:\n");
    for (int i = 0; i < 10; ++i) {
        int perfume_index = results[i].index;
        float similarity = results[i].similarity;
        printf("%d. Perfume ID: %s, Similarity: %.4f\n", i + 1, perfume_ids[perfume_index], similarity);
    }
}


void app_process_action() {
  printf("Inference...");
  TfLiteTensor* input_notes = interpreter->input(0);
  TfLiteTensor* input_accords = interpreter->input(1);
  TfLiteTensor* input_numerical = interpreter->input(2);
  TfLiteTensor* input_gender = interpreter->input(3);
  TfLiteTensor* input_brand = interpreter->input(4);
  TfLiteTensor* input_year = interpreter->input(5);

  static int32_t notes_encoded[NUM_NOTES];
  static int32_t accords_encoded[NUM_ACCORDS];
  static float numerical_features[NUM_NUMERICAL_FEATURES];

  memset(notes_encoded, 0, sizeof(notes_encoded));
  memset(accords_encoded, 0, sizeof(accords_encoded));
  memset(numerical_features, 0, sizeof(numerical_features));

  int32_t gender_code = test_user.gender;    // nếu bạn đã mã hoá gender thành số
  int32_t brand_code  = 0.0f;     // nếu có
  int32_t year_code   = 1.0f;      // nếu có

  multi_hot_encode_for_notes(test_user.notes, test_user.notes_count, notes_to_idx, NUM_NOTES, notes_encoded, NUM_NOTES);
  multi_hot_encode_for_accord(test_user.accords, test_user.accords_count, accords_to_idx, NUM_ACCORDS, accords_encoded, NUM_ACCORDS);


  numerical_features[0] = test_user.weighted_rating;
  numerical_features[1] = 100.0f;
  numerical_features[2] = test_user.weighted_rating;
  numerical_features[3] = static_cast<float>(test_user.longevity);
  numerical_features[4] = static_cast<float>(test_user.sillage);

  // --- copy notes (model expects int32 per dump) ---
  if (input_notes->type == kTfLiteInt32) {
    int32_t* dst = reinterpret_cast<int32_t*>(input_notes->data.raw);
    int dst_count = input_notes->bytes / sizeof(int32_t); // should be 193
    int copy_count = (NUM_NOTES < dst_count) ? NUM_NOTES : dst_count;
    for (int j = 0; j < copy_count; ++j) {
      dst[j] = notes_encoded[j];
    }
  } else if (input_notes->type == kTfLiteInt8 || input_notes->type == kTfLiteUInt8) {
    // nếu model dùng int8, phải chuyển scale/zero-point (không quên clamp)
    int8_t* dst = reinterpret_cast<int8_t*>(input_notes->data.raw);
    int dst_count = input_notes->bytes / sizeof(int8_t);
    int copy_count = (NUM_NOTES < dst_count) ? NUM_NOTES : dst_count;
    for (int j = 0; j < copy_count; ++j) {
      int v = notes_encoded[j];
      if (v < -128) v = -128;
      if (v > 127)  v = 127;
      dst[j] = static_cast<int8_t>(v);
    }
  } else {
    printf("Unsupported notes tensor type=%d\n", input_notes->type);
    return;
  }

  // --- copy accords (int32 per dump) ---
  if (input_accords->type == kTfLiteInt32) {
    int32_t* dst = reinterpret_cast<int32_t*>(input_accords->data.raw);
    int dst_count = input_accords->bytes / sizeof(int32_t); // 42
    int copy_count = (NUM_ACCORDS < dst_count) ? NUM_ACCORDS : dst_count;
    for (int j = 0; j < copy_count; ++j) dst[j] = accords_encoded[j];
  } else {
    // handle int8 case similarly if needed
    printf("Warning: input_accords type=%d not int32\n", input_accords->type);
  }

  // --- copy numerical features ---
  // numerical input in your dump had bytes=5; check type
  if (input_numerical->bytes < NUM_NUMERICAL_FEATURES) {
    // Might be quantized uint8 with one byte per feature -> need quantization
    // Here we attempt to write up to its capacity safely:
    int slot_count = input_numerical->bytes; // each slot is 1 byte in your dump
    // do quantization into uint8/int8 depending on type
    if (input_numerical->type == kTfLiteUInt8 || input_numerical->type == kTfLiteInt8) {
      uint8_t* dst = reinterpret_cast<uint8_t*>(input_numerical->data.raw);
      for (int j = 0; j < slot_count && j < NUM_NUMERICAL_FEATURES; ++j) {
        // quantize: use params.scale/zero_point
        float real = numerical_features[j];
        int32_t q = static_cast<int32_t>(round(real / input_numerical->params.scale + input_numerical->params.zero_point));
        if (q < 0) q = 0;
        if (q > 255) q = 255;
        dst[j] = static_cast<uint8_t>(q);
      }
    } else {
      printf("Unsupported numerical input type=%d bytes=%d\n", input_numerical->type, input_numerical->bytes);
    }
  } else {
    // enough capacity (unlikely in your dump), handle int8/int32 similarly
    if (input_numerical->type == kTfLiteInt32) {
      int32_t* dst = reinterpret_cast<int32_t*>(input_numerical->data.raw);
      int copy_count = (NUM_NUMERICAL_FEATURES < (input_numerical->bytes / 4)) ? NUM_NUMERICAL_FEATURES : (input_numerical->bytes / 4);
      for (int j = 0; j < copy_count; ++j) dst[j] = static_cast<int32_t>(numerical_features[j]);
    } else {
      // quantize as above if int8/uint8
    }
  }

  // --- copy single-value inputs (gender/brand/year) ---
  // They are 1-element int32 according to dump
  if (input_gender->bytes >= 4 && input_gender->type == kTfLiteInt32) {
    reinterpret_cast<int32_t*>(input_gender->data.raw)[0] = gender_code;
  } else {
    // if 1-byte quantized, quantize value
    if (input_gender->bytes == 1) {
      reinterpret_cast<int8_t*>(input_gender->data.raw)[0] =
          static_cast<int8_t>(round(gender_code / input_gender->params.scale + input_gender->params.zero_point));
    }
  }
  // brand
  if (input_brand->bytes >= 4 && input_brand->type == kTfLiteInt32) {
    reinterpret_cast<int32_t*>(input_brand->data.raw)[0] = brand_code;
  } else if (input_brand->bytes == 1) {
    reinterpret_cast<int8_t*>(input_brand->data.raw)[0] =
        static_cast<int8_t>(round(brand_code / input_brand->params.scale + input_brand->params.zero_point));
  }
  // year
  if (input_year->bytes >= 4 && input_year->type == kTfLiteInt32) {
    reinterpret_cast<int32_t*>(input_year->data.raw)[0] = year_code;
  } else if (input_year->bytes == 1) {
    reinterpret_cast<int8_t*>(input_year->data.raw)[0] =
        static_cast<int8_t>(round(year_code / input_year->params.scale + input_year->params.zero_point));
  }

  // Debug + Validate indices before invoke (you have debug_check_gather)
//  printf("ArenaUsed=%d\n", interpreter->arena_used_bytes());
//  debug_check_gather(interpreter);

  // Invoke
  TfLiteStatus status = interpreter->Invoke();
  if (status != kTfLiteOk) {
    printf("Inference failed");
  }

  // Predict
  TfLiteTensor* output = interpreter->output(0);
  handle_output(output);
}


void HardFault_Handler(void)
{
  printf("error: HardFault\n");
  while (1)
    ;
}
