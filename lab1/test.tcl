# Welcome message
puts "Welcome to the HW1 Demo!"

# Variable assignment and arithmetic
set a 5
set b 3
set sum [expr $a + $b]
set product [expr $a * $b]

puts "Variable a: $a"
puts "Variable b: $b"
puts "Sum: $sum"
puts "Product: $product"

# Loop (for)
puts "Counting from 1 to 5:"
for {set i 1} {$i <= 5} {incr i} {
    puts $i
}

# Procedure definition
proc square {num} {

    return [expr $num * $num]
}

# Procedure call
set result [square 4]
puts "Square of 4 is: $result"

# User input
puts -nonewline "Enter your age: "
flush stdout
gets stdin age

# Message based on user input
puts "You entered: $age"
if {$age < 18} {
    puts "You are a minor."
} else {
    puts "You are an adult."
}

# User input
puts -nonewline "Enter your age: "
flush stdout
gets stdin age

# Message based on user input
puts "You entered: $age"
if {$age < 18} {
    puts "You are a minor."
} else {
    puts "You are an adult."
}


puts "Demo complete!"

