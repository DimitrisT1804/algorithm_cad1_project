text = """three_counters/U85 three_counters/U59 three_counters/U84 three_counters/U87 three_counters/U86 three_counters/U54 {three_counters/\data_reg[4]} {three_counters/\counter2_reg[4]} three_counters/U89 three_counters/U55 three_counters/U88 three_counters/U56 three_counters/U57 three_counters/U50 {three_counters/\counter3_reg[2]} three_counters/U51 {three_counters/\data_reg[1]} three_counters/U52 three_counters/U53 {three_counters/\counter1_reg[0]} three_counters/U108 three_counters/U109 {three_counters/\data_reg[5]} three_counters/U100 three_counters/U101 {three_counters/\counter3_reg[5]} three_counters/U102 three_counters/U103 three_counters/U90 three_counters/U104 three_counters/U91 three_counters/U105 three_counters/U92 three_counters/U106 three_counters/U93 three_counters/U107 three_counters/U94 three_counters/U95 three_counters/U69 {three_counters/\counter1_reg[7]} three_counters/U96 three_counters/U68 three_counters/U97 three_counters/U67 three_counters/U98 three_counters/U66 three_counters/U99 three_counters/U65 three_counters/U64 three_counters/U63 three_counters/U29 three_counters/U29 three_counters/U62 three_counters/U28 three_counters/U28 three_counters/U61 {three_counters/\counter3_reg[1]} three_counters/U60 {three_counters/\data_reg[2]} {three_counters/\counter2_reg[5]} three_counters/U23 three_counters/U23 {three_counters/\counter3_reg[4]} three_counters/U22 three_counters/U22 {three_counters/\counter1_reg[3]} three_counters/U21 three_counters/U21 three_counters/U20 three_counters/U27 three_counters/U27 three_counters/U26 three_counters/U26 three_counters/U25 three_counters/U25 {three_counters/\counter1_reg[6]} three_counters/U24 three_counters/U24 {three_counters/\data_reg[6]} {three_counters/\counter2_reg[1]} three_counters/U78 three_counters/U79 three_counters/U76 {three_counters/\counter1_reg[2]} three_counters/U77 three_counters/U74 three_counters/U75 three_counters/U72 three_counters/U38 three_counters/U73 three_counters/U39 three_counters/U70 three_counters/U71 three_counters/U32 three_counters/U33 three_counters/U30 three_counters/U30 three_counters/U31 three_counters/U31 three_counters/U36 three_counters/U37 three_counters/U34 {three_counters/\data_reg[3]} three_counters/U35 {three_counters/\counter2_reg[6]} {three_counters/\counter3_reg[7]} three_counters/U122 three_counters/U123 three_counters/U120 three_counters/U121 {three_counters/\counter1_reg[5]} three_counters/U124 three_counters/U125 {three_counters/\counter2_reg[2]} three_counters/R_1 {three_counters/\counter3_reg[3]} three_counters/U49 three_counters/R_3 three_counters/U48 three_counters/R_2 {three_counters/\counter2_reg[7]} three_counters/U45 {three_counters/\counter1_reg[1]} three_counters/U44 three_counters/U47 three_counters/U46 three_counters/U119 three_counters/U41 three_counters/U118 three_counters/U40 three_counters/U43 three_counters/U42 three_counters/U111 {three_counters/\counter2_reg[3]} three_counters/U110 three_counters/U113 three_counters/U112 three_counters/U115 three_counters/U114 {three_counters/\data_reg[0]} three_counters/U117 three_counters/U116 {three_counters/\counter3_reg[6]} three_counters/U81 three_counters/U80 three_counters/U83 {three_counters/\counter1_reg[4]} three_counters/U82 three_counters/U58"""

# Split the text into a list of strings
strings_list = text.split()

# Check for duplicates
seen = set()
duplicates = set()

for string in strings_list:
    if string in seen:
        duplicates.add(string)
    else:
        seen.add(string)

# Print the result
if duplicates:
    print("Duplicate strings found:")
    for duplicate in duplicates:
        print(duplicate)
else:
    print("No duplicate strings.")
