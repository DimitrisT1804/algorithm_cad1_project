# Welcome message
puts "Welcome to the Tcl Shell Demo!"

# Variable assignment and arithmetic
set a 5
set b 3
set sum [expr $a + $b]
set product [expr $a * $b]

puts "Variable a: $a"
puts "Variable b: $b"
puts "Sum: $sum"
puts "Product: $product"

# Control structure (if-else)
set temperature 25
if {$temperature > 30} {
    puts "It's hot outside!"
} else {
    puts "It's a pleasant day."
}

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

# File operations
set filename "demo_file.txt"
set filecontent "This is a Tcl shell demo."

# Write to a file
set filehandle [open $filename w]
puts -nonewline $filehandle $filecontent
close $filehandle

# Read from a file
set filehandle [open $filename r]
set filedata [read $filehandle]
close $filehandle

puts "File content: $filedata"

# Delete the file
file delete $filename

puts "Demo complete!"

