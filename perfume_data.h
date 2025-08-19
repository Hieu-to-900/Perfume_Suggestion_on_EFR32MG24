/*
 * perfume_data.h
 *
 *  Created on: Aug 13, 2025
 *      Author: Admin
 */

#ifndef PERFUME_DATA_H_
#define PERFUME_DATA_H_

#include <cstdint>
#include <cmath>
#include <numeric>

// Quy ước mã hoá nhỏ gọn (1 byte mỗi field)
enum : uint8_t { G_MEN = 0, G_WOMEN = 1, G_UNISEX = 2 };
enum : uint8_t { L_LIGHT = 1, L_MODERATE = 2, L_STRONG = 3, L_VERY_STRONG = 4 };
enum : uint8_t { S_SOFT = 1, S_MODERATE = 2, S_HIGH = 3, S_HEAVY = 4 };

// Giới hạn mỗi lọ (chọn đủ lớn cho data của bạn)
static constexpr uint8_t MAX_NOTES_PER   = 22;  // xem lại theo dataset của bạn
static constexpr uint8_t MAX_ACCORDS_PER = 5;

struct PerfumeRO {
  uint8_t  gender;          // G_*
  uint8_t  longevity;       // L_*
  uint8_t  sillage;         // S_*
  uint8_t  notes_count;     // số phần tử hợp lệ trong notes[]
  uint8_t  accords_count;   // số phần tử hợp lệ trong accords[]
  float    weighted_rating; // giữ nguyên float
  // Các mảng con trỏ tới literal string. Vì toàn bộ struct là const,
  // các con trỏ và literal đều ở Flash (.rodata), không copy về RAM.
  const char* const notes[MAX_NOTES_PER];
  const char* const accords[MAX_ACCORDS_PER];
};

static const PerfumeRO all_perfume_data[50] = {

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 3,
    /* acc_cnt   */ 3,
    /* rating    */ 3.9997f,
    /* notes     */ { "jasmine sambac", "cashmeran", "amber" },
    /* accords   */ { "white floral", "amber", "floral" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 18,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9995f,
    /* notes     */ { "citruses", "bergamot", "lemon", "cedar", "mandarin orange", "iris", "patchouli", "vetiver", "jasmine", "rose", "vanilla", "incense", "leather", "opoponax", "civet", "sandalwood", "tonka bean", "musk" },
    /* accords   */ { "citrus", "amber", "woody", "vanilla", "balsamic" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_SOFT,
    /* notes_cnt */ 8,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9990f,
    /* notes     */ { "pink pepper", "silkwood blossom", "jasmine sambac", "whipped cream", "vanilla", "chestnut", "sandalwood", "cashmere wood" },
    /* accords   */ { "vanilla", "sweet", "woody", "balsamic", "powdery" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_LIGHT,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 10,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9990f,
    /* notes     */ { "honeysuckle", "tangerine", "rose", "jasmine", "tiare flower", "peony", "clementine", "musk", "sandalwood", "patchouli" },
    /* accords   */ { "white floral", "citrus", "fruity", "floral", "rose" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 22,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9990f,
    /* notes     */ { "lavender", "cinnamon", "pineapple", "juniper berries", "mint", "bergamot", "rosemary", "cardamom", "coriander", "orange blossom", "violet", "geranium", "jasmine", "raspberry", "vanilla", "sandalwood", "dark chocolate", "musk", "amber", "patchouli", "vetiver", "oakmoss" },
    /* accords   */ { "sweet", "fruity", "aromatic", "warm spicy", "woody" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 14,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9989f,
    /* notes     */ { "blood orange", "black currant", "clementine", "pear", "mandarin orange", "ginger", "ginger flower", "tuberose", "freesia", "hibiscus seed", "vanilla", "sandalwood", "musk", "california cedar" },
    /* accords   */ { "citrus", "fruity", "woody", "vanilla", "powdery" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_SOFT,
    /* notes_cnt */ 11,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9989f,
    /* notes     */ { "peach", "apricot", "pear", "black currant", "green apple", "sandalwood", "jasmine", "moss", "vanilla", "cedar", "musk" },
    /* accords   */ { "fruity", "powdery", "woody", "vanilla", "sweet" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 6,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9988f,
    /* notes     */ { "mint", "rose", "carnation", "cannabis", "vetiver", "amber" },
    /* accords   */ { "aromatic", "green", "rose", "floral", "cannabis" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 10,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9987f,
    /* notes     */ { "west indian bay", "pink pepper", "bergamot", "heliotrope", "almond", "jasmine", "vanilla", "tonka bean", "sandalwood", "patchouli" },
    /* accords   */ { "vanilla", "aromatic", "powdery", "sweet", "fresh spicy" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_SOFT,
    /* notes_cnt */ 5,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9985f,
    /* notes     */ { "litchi", "red currant", "rose", "vanilla", "vetiver" },
    /* accords   */ { "fruity", "rose", "fresh", "vanilla", "tropical" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 14,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9985f,
    /* notes     */ { "casablanca lily", "gardenia", "jasmine", "mandarin orange", "incense", "fruity notes", "white pepper", "tobacco", "peach", "rose", "sandalwood", "patchouli", "amber", "vanilla" },
    /* accords   */ { "white floral", "fruity", "amber", "warm spicy", "woody" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 9,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9983f,
    /* notes     */ { "violet leaf", "artemisia", "mandarin orange", "white pepper", "cedar", "nutmeg", "white musk", "tonka bean", "vetiver" },
    /* accords   */ { "fresh spicy", "ozonic", "musky", "aquatic", "woody" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 8,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9981f,
    /* notes     */ { "grapefruit", "lemon", "pink pepper", "mango", "coconut", "rice", "vetiver", "tonka bean" },
    /* accords   */ { "sweet", "citrus", "tropical", "aromatic", "coconut" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 19,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9980f,
    /* notes     */ { "musk", "lily", "ylang-ylang", "galbanum", "basil", "musk", "lily", "jasmine", "rose", "musk", "iris", "jasmine", "oakmoss", "amber", "vanilla", "rose", "vetiver", "patchouli", "peach" },
    /* accords   */ { "musky", "powdery", "white floral", "iris", "earthy" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 15,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9976f,
    /* notes     */ { "artemisia", "caraway", "lemon", "bergamot", "grapefruit", "pine tree", "sandalwood", "patchouli", "carnation", "cyclamen", "jasmine", "oakmoss", "tobacco", "leather", "amber" },
    /* accords   */ { "woody", "aromatic", "mossy", "earthy", "tobacco" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 6,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9974f,
    /* notes     */ { "bergamot", "apple", "black tea", "black amber", "musk", "oakmoss" },
    /* accords   */ { "green", "citrus", "fresh", "fresh spicy", "amber" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 14,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9973f,
    /* notes     */ { "violet leaf", "lotus", "bergamot", "white pear", "vetyver", "woodsy notes", "cashmere wood", "sandalwood", "amber", "cypriol oil or nagarmotha", "white honey", "musk", "benzoin", "tonka bean" },
    /* accords   */ { "woody", "ozonic", "amber", "aquatic", "aromatic" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 13,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9972f,
    /* notes     */ { "powdery notes", "rose", "ylang-ylang", "jasmine", "rose", "tonka bean", "floral notes", "jasmine", "heliotrope", "musk", "rose", "sugar", "jasmine" },
    /* accords   */ { "powdery", "vanilla", "sweet", "floral", "musky" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 12,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9972f,
    /* notes     */ { "orange", "bergamot", "violet leaf", "basil", "pink pepper", "cinnamon", "nutmeg", "cardamom", "musk", "sandalwood", "woodsy notes", "vetiver" },
    /* accords   */ { "citrus", "fresh spicy", "warm spicy", "woody", "aromatic" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 13,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9972f,
    /* notes     */ { "beeswax", "ginger", "orange", "mimosa", "broom", "heliotrope", "orange blossom", "vanilla", "benzoin", "tonka bean", "sandalwood", "labdanum", "musk" },
    /* accords   */ { "vanilla", "beeswax", "honey", "powdery", "amber" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 14,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9971f,
    /* notes     */ { "bitter orange", "mandarin orange", "turkish rose", "grasse rose", "musk", "egyptian jasmine", "jasmine sambac", "jasmine", "cashmere wood", "patchouli", "madagascar vanilla", "acácia", "sandalwood", "cedar" },
    /* accords   */ { "woody", "rose", "musky", "citrus", "floral" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_LIGHT,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 7,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9968f,
    /* notes     */ { "candy apple", "sugar", "tea", "jasmine", "rose", "patchouli", "musk" },
    /* accords   */ { "sweet", "fruity", "fresh", "green", "white floral" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_LIGHT,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 19,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9968f,
    /* notes     */ { "juniper", "cypress", "lavender", "bergamot", "carnation", "galbanum", "elemi resin", "vetiver", "ylang-ylang", "rose", "lily-of-the-valley", "cassia", "oakmoss", "pine tree", "cedar", "patchouli", "musk", "pepper", "sandalwood" },
    /* accords   */ { "woody", "aromatic", "fresh spicy", "earthy", "green" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 6,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9967f,
    /* notes     */ { "calabrian bergamot", "petitgrain", "ginger", "cardamom", "white musk", "woody notes" },
    /* accords   */ { "citrus", "warm spicy", "aromatic", "fresh spicy", "woody" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 5,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9966f,
    /* notes     */ { "gardenia", "cinnamon leaf", "green notes", "white musk", "sandalwood" },
    /* accords   */ { "green", "white floral", "warm spicy", "musky", "powdery" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 14,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9966f,
    /* notes     */ { "plum", "apple", "grapefruit", "bergamot", "cinnamon", "pink pepper", "cardamom", "juniper", "vanilla", "rum", "musk", "sandalwood", "cedar", "labdanum" },
    /* accords   */ { "fruity", "warm spicy", "woody", "sweet", "vanilla" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 17,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9966f,
    /* notes     */ { "narcissus", "lily-of-the-valley", "violet", "jasmine", "rose", "peach", "bergamot", "aldehydes", "rosemary", "tuberose", "civet", "musk", "vetiver", "sandalwood", "bamboo", "amber", "tonka bean" },
    /* accords   */ { "white floral", "fresh", "aldehydic", "woody", "powdery" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_SOFT,
    /* notes_cnt */ 7,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9965f,
    /* notes     */ { "grapefruit", "bergamot", "nutmeg", "leather", "vetiver", "virginia cedar", "ambergris" },
    /* accords   */ { "citrus", "fresh spicy", "woody", "aromatic", "leather" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 13,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9963f,
    /* notes     */ { "mandarin orange", "water mint", "ginger", "basil", "lotus", "incense", "cedar", "nutmeg", "exotic woods", "musk", "violet leaf", "suede", "amber" },
    /* accords   */ { "green", "fresh spicy", "citrus", "aromatic", "warm spicy" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_SOFT,
    /* notes_cnt */ 10,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9962f,
    /* notes     */ { "mint", "geranium", "star anise", "clove", "cinnamon", "musk", "sandalwood", "incense", "styrax", "benzoin" },
    /* accords   */ { "aromatic", "fresh spicy", "green", "warm spicy", "rose" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 11,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9960f,
    /* notes     */ { "ceylon cinnamon", "saffron", "suede", "agarwood (oud)", "madagascar ylang-ylang", "jasmine", "tahitian vanilla", "australian sandalwood", "tobacco leaf", "musk", "olibanum" },
    /* accords   */ { "vanilla", "powdery", "warm spicy", "leather", "musky" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 12,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9959f,
    /* notes     */ { "whipped cream", "black currant", "red fruits", "dried plum", "orange blossom", "madagascar vanilla", "jasmine sambac", "chocolate", "caramel", "salt", "patchouli", "ambertonic" },
    /* accords   */ { "sweet", "fruity", "vanilla", "chocolate", "lactonic" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_VERY_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 7,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9959f,
    /* notes     */ { "orange blossom", "black pepper", "whiskey", "laurels", "amber", "ambergris", "salt" },
    /* accords   */ { "amber", "fresh spicy", "whiskey", "animalic", "white floral" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_LIGHT,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 9,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9959f,
    /* notes     */ { "green mandarin", "bergamot", "lavender", "violet", "calone", "leather", "cashmere wood", "tonka bean", "vetiver" },
    /* accords   */ { "citrus", "leather", "woody", "aromatic", "lavender" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 14,
    /* acc_cnt   */ 4,
    /* rating    */ 3.9958f,
    /* notes     */ { "mango", "passionfruit", "raspberry", "passion flower", "orange", "magnolia", "white flowers", "white peach", "red berries", "hibiscus", "rose hip", "musk", "sandalwood", "white woods" },
    /* accords   */ { "fruity", "sweet", "tropical", "floral" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 10,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9957f,
    /* notes     */ { "amalfi lemon", "lotus", "basil", "ambrette (musk mallow)", "olive blossom", "magnolia", "jasmine", "musk", "benzoin", "amber" },
    /* accords   */ { "floral", "citrus", "aromatic", "fresh spicy", "musky" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 10,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9956f,
    /* notes     */ { "pineapple", "hyacinth", "iris", "jasmine", "pink pepper", "musk", "vetiver", "amber", "vanilla", "patchouli" },
    /* accords   */ { "musky", "powdery", "sweet", "earthy", "fruity" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 15,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9956f,
    /* notes     */ { "raspberry", "black currant", "pear", "orange", "bergamot", "may rose", "bulgarian rose", "peony", "datura", "freesia", "vanilla", "patchouli", "white musk", "benzoin", "cashmeran" },
    /* accords   */ { "fruity", "rose", "floral", "sweet", "vanilla" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 9,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9955f,
    /* notes     */ { "amalfi lemon", "bergamot", "pepper", "petitgrain", "sage", "lavender", "oakmoss", "indonesian patchouli leaf", "musk" },
    /* accords   */ { "aromatic", "citrus", "fresh spicy", "herbal", "lavender" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 9,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9954f,
    /* notes     */ { "pear", "mandarin orange", "jasmine", "rose", "peony", "jasmine sambac", "ambroxan", "orris", "benzoin" },
    /* accords   */ { "rose", "floral", "fresh", "fruity", "citrus" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 3,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9954f,
    /* notes     */ { "pear", "jasmine", "honey" },
    /* accords   */ { "honey", "sweet", "fruity", "white floral", "floral" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_STRONG,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 14,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9953f,
    /* notes     */ { "geranium", "rosebay willowherb", "indian tuberose", "ylang-ylang", "jasmine sambac", "egyptian jasmine", "iris", "madagascar vanilla", "benzoin", "sandalwood", "cashmeran", "singapore patchouli", "incense", "cypress" },
    /* accords   */ { "white floral", "tuberose", "woody", "yellow floral", "vanilla" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 4,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9953f,
    /* notes     */ { "iris", "rose", "musk", "amber" },
    /* accords   */ { "powdery", "musky", "iris", "violet", "rose" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 15,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9953f,
    /* notes     */ { "aldehydes", "neroli", "peach", "bergamot", "ylang-ylang", "rose", "jasmine", "geranium", "orchid", "musk", "vanille", "sandalwood", "tonka bean", "vetiver", "virginia cedar" },
    /* accords   */ { "woody", "aldehydic", "powdery", "sweet", "rose" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 9,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9953f,
    /* notes     */ { "coriander extract", "plum", "bergamot", "jasmine", "orange blossom", "tunisian neroli", "madagascar vanilla", "myrrh", "peru balsam" },
    /* accords   */ { "amber", "vanilla", "balsamic", "sweet", "white floral" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_MODERATE,
    /* notes_cnt */ 9,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9952f,
    /* notes     */ { "cherry", "raspberry", "big strawberry", "jasmine", "heliotrope", "lily-of-the-valley", "vanilla", "amber", "sandalwood" },
    /* accords   */ { "sweet", "fruity", "cherry", "vanilla", "powdery" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_SOFT,
    /* notes_cnt */ 3,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9952f,
    /* notes     */ { "orange blossom", "coffee", "vanilla" },
    /* accords   */ { "vanilla", "coffee", "white floral", "warm spicy", "sweet" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HIGH,
    /* notes_cnt */ 9,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9952f,
    /* notes     */ { "gardenia", "ylang-ylang", "mirabilis", "rose", "violet", "geranium", "dried plum", "musk", "woody notes" },
    /* accords   */ { "rose", "powdery", "floral", "fruity", "violet" }
  },

  {
    /* gender    */ G_WOMEN,
    /* longevity */ L_STRONG,
    /* sillage   */ S_HEAVY,
    /* notes_cnt */ 13,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9952f,
    /* notes     */ { "coriander", "pear", "bergamot", "red berries", "pistachio", "almond", "violet", "mimosa", "peony", "suede", "vanilla", "patchouli", "musk" },
    /* accords   */ { "powdery", "aromatic", "sweet", "fruity", "woody" }
  },

  {
    /* gender    */ G_UNISEX,
    /* longevity */ L_MODERATE,
    /* sillage   */ S_SOFT,
    /* notes_cnt */ 17,
    /* acc_cnt   */ 5,
    /* rating    */ 3.9951f,
    /* notes     */ { "birch leaf", "black pepper", "green mandarin", "pink pepper", "mint", "lemon", "leather", "patchouli", "texas cedar", "violet leaf", "coriander", "tonka bean", "incense", "amber", "vanila", "sandalwood", "vetiver" },
    /* accords   */ { "amber", "leather", "smoky", "woody", "warm spicy" }
  },
};

static inline const PerfumeRO& perfume_at(uint8_t i) {
  // i < 50 là trách nhiệm của người gọi (hoặc bạn có thể assert nhẹ)
  return all_perfume_data[i];
}


struct AccordEntry {
    const char* name;
    int8_t index;
};

static const AccordEntry accords_to_idx[] = {
    {"aldehydic", 0}, {"amber", 1}, {"animalic", 2}, {"aquatic", 3}, {"aromatic", 4},
    {"balsamic", 5}, {"beeswax", 6}, {"cannabis", 7}, {"cherry", 8}, {"chocolate", 9},
    {"citrus", 10}, {"coconut", 11}, {"coffee", 12}, {"earthy", 13}, {"floral", 14},
    {"fresh", 15}, {"fresh spicy", 16}, {"fruity", 17}, {"green", 18}, {"herbal", 19},
    {"honey", 20}, {"iris", 21}, {"lactonic", 22}, {"lavender", 23}, {"leather", 24},
    {"mossy", 25}, {"musky", 26}, {"ozonic", 27}, {"powdery", 28}, {"rose", 29},
    {"smoky", 30}, {"sweet", 31}, {"tobacco", 32}, {"tropical", 33}, {"tuberose", 34},
    {"vanilla", 35}, {"violet", 36}, {"warm spicy", 37}, {"whiskey", 38}, {"white floral", 39},
    {"woody", 40}, {"yellow floral", 41}
};



struct RatingEntry {
    const char* name;
    float value;
};

static const RatingEntry longevity_map[] = {
    {"Very Strong", 4.0f},
    {"Strong", 3.0f},
    {"Moderate", 2.0f},
    {"Light", 1.0f}
};

static const RatingEntry sillage_map[] = {
    {"Heavy", 4.0f},
    {"High", 3.0f},
    {"Moderate", 2.0f},
    {"Soft", 1.0f}
};


struct GenderEntry {
  const char* gender;
  int8_t index;
};

static const GenderEntry gender_map[] = {
    {"men", 0},
    {"women", 1},
    {"unisex", 2}
};



struct NoteIndex {
    const char* name;
    int32_t index;
};

// Mảng dữ liệu bất biến, nằm ở Flash (.rodata)
constexpr NoteIndex notes_to_idx[] = {
    {"acácia", 0}, {"agarwood (oud)", 1}, {"aldehydes", 2}, {"almond", 3}, {"amalfi lemon", 4},
    {"amber", 5}, {"ambergris", 6}, {"ambertonic", 7}, {"ambrette (musk mallow)", 8}, {"ambroxan", 9},
    {"apple", 10}, {"apricot", 11}, {"artemisia", 12}, {"australian sandalwood", 13}, {"bamboo", 14},
    {"basil", 15}, {"beeswax", 16}, {"benzoin", 17}, {"bergamot", 18}, {"big strawberry", 19},
    {"birch leaf", 20}, {"bitter orange", 21}, {"black amber", 22}, {"black currant", 23}, {"black pepper", 24},
    {"black tea", 25}, {"blood orange", 26}, {"broom", 27}, {"bulgarian rose", 28}, {"calabrian bergamot", 29},
    {"california cedar", 30}, {"calone", 31}, {"candy apple", 32}, {"cannabis", 33}, {"caramel", 34},
    {"caraway", 35}, {"cardamom", 36}, {"carnation", 37}, {"casablanca lily", 38}, {"cashmeran", 39},
    {"cashmere wood", 40}, {"cassia", 41}, {"cedar", 42}, {"ceylon cinnamon", 43}, {"cherry", 44},
    {"chestnut", 45}, {"chocolate", 46}, {"cinnamon", 47}, {"cinnamon leaf", 48}, {"citruses", 49},
    {"civet", 50}, {"clementine", 51}, {"clove", 52}, {"coconut", 53}, {"coffee", 54},
    {"coriander", 55}, {"coriander extract", 56}, {"cyclamen", 57}, {"cypress", 58}, {"cypriol oil or nagarmotha", 59},
    {"dark chocolate", 60}, {"datura", 61}, {"dried plum", 62}, {"egyptian jasmine", 63}, {"elemi resin", 64},
    {"exotic woods", 65}, {"floral notes", 66}, {"freesia", 67}, {"fruity notes", 68}, {"galbanum", 69},
    {"gardenia", 70}, {"geranium", 71}, {"ginger", 72}, {"ginger flower", 73}, {"grapefruit", 74},
    {"grasse rose", 75}, {"green apple", 76}, {"green mandarin", 77}, {"green notes", 78}, {"heliotrope", 79},
    {"hibiscus", 80}, {"hibiscus seed", 81}, {"honey", 82}, {"honeysuckle", 83}, {"hyacinth", 84},
    {"incense", 85}, {"indian tuberose", 86}, {"indonesian patchouli leaf", 87}, {"iris", 88}, {"jasmine", 89},
    {"jasmine sambac", 90}, {"juniper", 91}, {"juniper berries", 92}, {"labdanum", 93}, {"laurels", 94},
    {"lavender", 95}, {"leather", 96}, {"lemon", 97}, {"lily", 98}, {"lily-of-the-valley", 99},
    {"litchi", 100}, {"lotus", 101}, {"madagascar vanilla", 102}, {"madagascar ylang-ylang", 103}, {"magnolia", 104},
    {"mandarin orange", 105}, {"mango", 106}, {"may rose", 107}, {"mimosa", 108}, {"mint", 109},
    {"mirabilis", 110}, {"moss", 111}, {"musk", 112}, {"myrrh", 113}, {"narcissus", 114},
    {"neroli", 115}, {"nutmeg", 116}, {"oakmoss", 117}, {"olibanum", 118}, {"olive blossom", 119},
    {"opoponax", 120}, {"orange", 121}, {"orange blossom", 122}, {"orchid", 123}, {"orris", 124},
    {"passion flower", 125}, {"passionfruit", 126}, {"patchouli", 127}, {"peach", 128}, {"pear", 129},
    {"peony", 130}, {"pepper", 131}, {"peru balsam", 132}, {"petitgrain", 133}, {"pine tree", 134},
    {"pineapple", 135}, {"pink pepper", 136}, {"pistachio", 137}, {"plum", 138}, {"powdery notes", 139},
    {"raspberry", 140}, {"red berries", 141}, {"red currant", 142}, {"red fruits", 143}, {"rice", 144},
    {"rose", 145}, {"rose hip", 146}, {"rosebay willowherb", 147}, {"rosemary", 148}, {"rum", 149},
    {"saffron", 150}, {"sage", 151}, {"salt", 152}, {"sandalwood", 153}, {"silkwood blossom", 154},
    {"singapore patchouli", 155}, {"star anise", 156}, {"styrax", 157}, {"suede", 158}, {"sugar", 159},
    {"tahitian vanilla", 160}, {"tangerine", 161}, {"tea", 162}, {"texas cedar", 163}, {"tiare flower", 164},
    {"tobacco", 165}, {"tobacco leaf", 166}, {"tonka bean", 167}, {"tuberose", 168}, {"tunisian neroli", 169},
    {"turkish rose", 170}, {"vanila", 171}, {"vanilla", 172}, {"vanille", 173}, {"vetiver", 174},
    {"vetyver", 175}, {"violet", 176}, {"violet leaf", 177}, {"virginia cedar", 178}, {"water mint", 179},
    {"west indian bay", 180}, {"whipped cream", 181}, {"whiskey", 182}, {"white flowers", 183}, {"white honey", 184},
    {"white musk", 185}, {"white peach", 186}, {"white pear", 187}, {"white pepper", 188}, {"white woods", 189},
    {"woodsy notes", 190}, {"woody notes", 191}, {"ylang-ylang", 192}
};

// Định nghĩa cấu trúc để lưu trữ kết quả và chỉ mục
struct SimilarityResult {
    int index;
    float similarity;
};


// Mảng chứa các mã nước hoa (PID_01)
static const char* perfume_ids[] = {
    "P001", "P002", "P003", "P004", "P005", "P006", "P007", "P008", "P009", "P010",
    "P011", "P012", "P013", "P014", "P015", "P016", "P017", "P018", "P019", "P020",
    "P021", "P022", "P023", "P024", "P025", "P026", "P027", "P028", "P029", "P030",
    "P031", "P032", "P033", "P034", "P035", "P036", "P037", "P038", "P039", "P040",
    "P041", "P042", "P043", "P044", "P045", "P046", "P047", "P048", "P049", "P050"
};





































#endif /* PERFUME_DATA_H_ */
