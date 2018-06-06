src/./matrix_power src/input.txt src/output.txt
output=$(echo -e "Type\tWidth\tSparsity\tPower\tTime(s)")
output=${output}"\n"$(cat src/output.txt)
echo -e "$output" | sed 's/\,/\t/g' |  column -t
