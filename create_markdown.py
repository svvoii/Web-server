import re

# Open the C++ file and read its content
with open('external_functions_qref_ex.cpp', 'r') as cpp_file:
	cpp_content = cpp_file.readlines()

# Initialize the markdown content with a header
md_content = ['# Function Quick Reference\n\n']

# Initialize a list to hold the function names
functions = []

# Initialize a variable to hold the current function name
current_function = None

# Initialize a flag to indicate whether we're in a function description
in_description = False

# Iterate over each line in the C++ file
for line in cpp_content:
	# If the line contains a function name, add it to the list
	if re.match(r'\d+\)', line):
		current_function = line.split(')')[1].strip()
		functions.append(current_function)
		in_description = True
	elif in_description and re.match(r'< < < < < < < < < < < < < < < < < < <', line):
		# If the line contains the end of a function description, stop adding lines to the description
		in_description = False

	if in_description:
		# Convert the content to markdown format and add it to the description
		md_line = line.replace('/*', '').replace('*/', '').replace('* * ', '')
		md_content.append(md_line)

# Create a table of contents at the top of the document
toc = ['## Table of Contents\n']
for i, function_name in enumerate(functions, 1):
	# Remove special characters from the function name when creating the anchor
	anchor = re.sub(r'\W+', '', function_name.lower())
	toc.append(f'{i}. [{function_name}](#{anchor})\n')

# Insert the table of contents at the beginning of the markdown content
md_content = toc + md_content

# Write the markdown content to a new file
with open('functions_reference.md', 'w') as md_file:
	md_file.writelines(md_content)

""" import re

# Open the C++ file and read its content
with open('external_functions_qref_ex.cpp', 'r') as cpp_file:
	cpp_content = cpp_file.readlines()

# Initialize the markdown content with a header
md_content = ['# Function Quick Reference\n\n']

# Initialize a list to hold the function names
functions = []

# Initialize a variable to hold the current function name
current_function = None

# Iterate over each line in the C++ file
for line in cpp_content:
	# If the line contains a function name, add it to the list
	if re.match(r'\d+\)', line):
		current_function = line.split(')')[1].strip()
		functions.append(current_function)
	elif current_function and re.match(r'\* \* `', line):
		# If the line contains the start of a function description, add a markdown header
		md_content.append(f'## {current_function}\n')
		md_content.append(line.replace('* * `', '`'))
		current_function = None
	elif not current_function:
		# Convert the content to markdown format
		md_line = line.replace('/*', '').replace('*/', '').replace('* * ', '')
		md_content.append(md_line)

# Create a table of contents at the top of the document
toc = ['## Table of Contents\n']
for i, function_name in enumerate(functions, 1):
	toc.append(f'{i}. [{function_name}](#{function_name.lower().replace(" ", "-")})\n')

# Insert the table of contents at the beginning of the markdown content
md_content = toc + md_content

# Write the markdown content to a new file
with open('functions_reference.md', 'w') as md_file:
	md_file.writelines(md_content)
 """
