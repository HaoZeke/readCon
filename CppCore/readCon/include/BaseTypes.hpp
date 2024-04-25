#pragma once
// MIT License
// Copyright 2023--present Rohit Goswami <HaoZeke>

#include <array>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

namespace yodecon::types {

/**
 * @struct AtomDatum
 * @brief Represents a single atom's data in a molecular structure.
 *
 * This structure contains the spatial coordinates, chemical symbol, fixed
 * status, and a unique identifier for an atom within a simulation frame. It
 * provides constructors for initializing these properties directly.
 *
 * @note Mass is not included as it is typically defined in the container
 * structure's metadata. This avoids redundancy and keeps the structure
 * lightweight. Also, mass shouldn't be hard-coded into the structure, consider
 * isotopes.
 *
 * Example of creating an AtomDatum:
 * @code
 * AtomDatum atom("C", 1.0, 2.0, 3.0, false, 123);
 * @endcode
 */
struct AtomDatum {
  std::string symbol; ///< Chemical symbol of the atom (e.g., "C" for Carbon).
  double x, y,
      z;         ///< Cartesian coordinates of the atom in the simulation space.
  bool is_fixed; ///< Flag indicating if the atom's position is fixed during the
                 ///< simulation.
  int atom_id;   ///< Unique identifier for the atom within the simulation.
  /**
   * @brief Default constructor initializing members to default values.
   */
  AtomDatum() : symbol{"NaN"}, x{0}, y{0}, z{0}, is_fixed{false}, atom_id{0} {}
  /**
   * @brief Parameterized constructor for setting all properties of the atom.
   * @param a_symbol Chemical symbol of the atom.
   * @param a_x X-coordinate of the atom.
   * @param a_y Y-coordinate of the atom.
   * @param a_z Z-coordinate of the atom.
   * @param a_is_fixed Boolean flag to set the atom's fixed status.
   * @param a_atom_id Unique identifier for the atom.
   */
  AtomDatum(std::string a_symbol, double a_x, double a_y, double a_z,
            bool a_is_fixed, int a_atom_id)
      : symbol{a_symbol}, x{a_x}, y{a_y}, z{a_z}, is_fixed{a_is_fixed},
        atom_id{a_atom_id} {}
  //  Not sure I want / need functions here (C compatility considerations)
  //   Then again there are constructors already
  //   bool operator==(const AtomDatum &rhs) const {
  //   return symbol == rhs.symbol && std::abs(x - rhs.x) < 1e-9 &&
  //          std::abs(y - rhs.y) < 1e-9 && std::abs(z - rhs.z) < 1e-9 &&
  //          is_fixed == rhs.is_fixed && atom_id == rhs.atom_id;
  // }
};

/**
 * @struct ConFrame
 * @brief Structure to store configuration frame data in a compact form.
 *
 * This structure is designed to hold all relevant information about a
 * configuration frame, including header information, box dimensions and angles,
 * atom types, and atom-specific data.
 *
 * @note Consider providing a constructor that initializes data from a file.
 *
 * @todo Implement constructor taking a file path for initialization.
 * @todo Remove the default constructor to enforce initialization integrity.
 */
struct ConFrame {
  std::array<std::string, 2> prebox_header;
  std::array<double, 3> boxl;
  std::array<double, 3> angles;
  std::array<std::string, 2> postbox_header;
  size_t natm_types;
  std::vector<size_t> natms_per_type;
  std::vector<double> masses_per_type;
  std::vector<AtomDatum> atom_data;
};

/**
 * @struct ConFrameVec
 * @brief Structure to store expanded configuration frame data with separate
 * vectors for each attribute.
 *
 * The ConFrameVec structure is an alternative representation of configuration
 * frame data, where each attribute of the atoms is stored in separate vectors.
 * This structure is useful for scenarios where operations on individual
 * attributes across all atoms are needed.
 *
 * @note This structure provides an expanded view of the data, contrasting with
 * the compact nature of ConFrame, making it suitable for operations that
 * require direct access to specific atom properties without the need to iterate
 * through an array of atom data.
 */
struct ConFrameVec {
  std::array<std::string, 2> prebox_header;
  std::array<double, 3> boxl;
  std::array<double, 3> angles;
  std::array<std::string, 2> postbox_header;
  size_t natm_types;
  std::vector<size_t> natms_per_type;
  std::vector<double> masses_per_type;
  std::vector<std::string> symbol;
  std::vector<double> x, y, z;
  std::vector<bool> is_fixed;
  std::vector<int> atom_id;
};

namespace known_info {
/**
 * @brief Maps atomic symbols to their respective atomic numbers.
 *
 * This map is used to quickly convert from an atomic symbol to its
 * corresponding atomic number. Each entry pairs a string representing the
 * chemical symbol with its integer atomic number, facilitating O(1) lookup time
 * for any of the 118 recognized chemical elements.
 *
 * Example usage:
 * @code
 * int atomicNumber = AtomicNumbers["C"]; // Retrieves the atomic number for
 * Carbon
 * @endcode
 */
const std::unordered_map<std::string, int> AtomicNumbers = {
    {"H", 1},    {"He", 2},   {"Li", 3},   {"Be", 4},   {"B", 5},
    {"C", 6},    {"N", 7},    {"O", 8},    {"F", 9},    {"Ne", 10},
    {"Na", 11},  {"Mg", 12},  {"Al", 13},  {"Si", 14},  {"P", 15},
    {"S", 16},   {"Cl", 17},  {"Ar", 18},  {"K", 19},   {"Ca", 20},
    {"Sc", 21},  {"Ti", 22},  {"V", 23},   {"Cr", 24},  {"Mn", 25},
    {"Fe", 26},  {"Co", 27},  {"Ni", 28},  {"Cu", 29},  {"Zn", 30},
    {"Ga", 31},  {"Ge", 32},  {"As", 33},  {"Se", 34},  {"Br", 35},
    {"Kr", 36},  {"Rb", 37},  {"Sr", 38},  {"Y", 39},   {"Zr", 40},
    {"Nb", 41},  {"Mo", 42},  {"Tc", 43},  {"Ru", 44},  {"Rh", 45},
    {"Pd", 46},  {"Ag", 47},  {"Cd", 48},  {"In", 49},  {"Sn", 50},
    {"Sb", 51},  {"Te", 52},  {"I", 53},   {"Xe", 54},  {"Cs", 55},
    {"Ba", 56},  {"La", 57},  {"Ce", 58},  {"Pr", 59},  {"Nd", 60},
    {"Pm", 61},  {"Sm", 62},  {"Eu", 63},  {"Gd", 64},  {"Tb", 65},
    {"Dy", 66},  {"Ho", 67},  {"Er", 68},  {"Tm", 69},  {"Yb", 70},
    {"Lu", 71},  {"Hf", 72},  {"Ta", 73},  {"W", 74},   {"Re", 75},
    {"Os", 76},  {"Ir", 77},  {"Pt", 78},  {"Au", 79},  {"Hg", 80},
    {"Tl", 81},  {"Pb", 82},  {"Bi", 83},  {"Po", 84},  {"At", 85},
    {"Rn", 86},  {"Fr", 87},  {"Ra", 88},  {"Ac", 89},  {"Th", 90},
    {"Pa", 91},  {"U", 92},   {"Np", 93},  {"Pu", 94},  {"Am", 95},
    {"Cm", 96},  {"Bk", 97},  {"Cf", 98},  {"Es", 99},  {"Fm", 100},
    {"Md", 101}, {"No", 102}, {"Lr", 103}, {"Rf", 104}, {"Db", 105},
    {"Sg", 106}, {"Bh", 107}, {"Hs", 108}, {"Mt", 109}, {"Ds", 110},
    {"Rg", 111}, {"Cn", 112}, {"Nh", 113}, {"Fl", 114}, {"Mc", 115},
    {"Lv", 116}, {"Ts", 117}, {"Og", 118}};

/**
 * @brief Maps atomic numbers to their respective atomic symbols.
 *
 * This map provides a fast access route from an atomic number to its
 * corresponding symbol. Maintaining a separate map for reverse lookups ensures
 * O(1) time complexity, which is crucial for performance-critical applications
 * where reverse lookups are frequent.
 *
 * Example usage:
 * @code
 * std::string symbol = AtomicSymbols[6]; // Retrieves the symbol for atomic
 * number 6, which is Carbon
 * @endcode
 *
 * @note Maintaining a second map may seem redundant, but it avoids the O(N)
 * time complexity of searching through a reverse map. This design decision
 * optimizes performance, particularly during write-out operations where
 * multiple lookups may occur.
 */
const std::unordered_map<int, std::string> AtomicSymbols = {
    {1, "H"},    {2, "He"},   {3, "Li"},   {4, "Be"},   {5, "B"},
    {6, "C"},    {7, "N"},    {8, "O"},    {9, "F"},    {10, "Ne"},
    {11, "Na"},  {12, "Mg"},  {13, "Al"},  {14, "Si"},  {15, "P"},
    {16, "S"},   {17, "Cl"},  {18, "Ar"},  {19, "K"},   {20, "Ca"},
    {21, "Sc"},  {22, "Ti"},  {23, "V"},   {24, "Cr"},  {25, "Mn"},
    {26, "Fe"},  {27, "Co"},  {28, "Ni"},  {29, "Cu"},  {30, "Zn"},
    {31, "Ga"},  {32, "Ge"},  {33, "As"},  {34, "Se"},  {35, "Br"},
    {36, "Kr"},  {37, "Rb"},  {38, "Sr"},  {39, "Y"},   {40, "Zr"},
    {41, "Nb"},  {42, "Mo"},  {43, "Tc"},  {44, "Ru"},  {45, "Rh"},
    {46, "Pd"},  {47, "Ag"},  {48, "Cd"},  {49, "In"},  {50, "Sn"},
    {51, "Sb"},  {52, "Te"},  {53, "I"},   {54, "Xe"},  {55, "Cs"},
    {56, "Ba"},  {57, "La"},  {58, "Ce"},  {59, "Pr"},  {60, "Nd"},
    {61, "Pm"},  {62, "Sm"},  {63, "Eu"},  {64, "Gd"},  {65, "Tb"},
    {66, "Dy"},  {67, "Ho"},  {68, "Er"},  {69, "Tm"},  {70, "Yb"},
    {71, "Lu"},  {72, "Hf"},  {73, "Ta"},  {74, "W"},   {75, "Re"},
    {76, "Os"},  {77, "Ir"},  {78, "Pt"},  {79, "Au"},  {80, "Hg"},
    {81, "Tl"},  {82, "Pb"},  {83, "Bi"},  {84, "Po"},  {85, "At"},
    {86, "Rn"},  {87, "Fr"},  {88, "Ra"},  {89, "Ac"},  {90, "Th"},
    {91, "Pa"},  {92, "U"},   {93, "Np"},  {94, "Pu"},  {95, "Am"},
    {96, "Cm"},  {97, "Bk"},  {98, "Cf"},  {99, "Es"},  {100, "Fm"},
    {101, "Md"}, {102, "No"}, {103, "Lr"}, {104, "Rf"}, {105, "Db"},
    {106, "Sg"}, {107, "Bh"}, {108, "Hs"}, {109, "Mt"}, {110, "Ds"},
    {111, "Rg"}, {112, "Cn"}, {113, "Nh"}, {114, "Fl"}, {115, "Mc"},
    {116, "Lv"}, {117, "Ts"}, {118, "Og"}};

} // namespace known_info
} // namespace yodecon::types
