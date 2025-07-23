main.cpp instantiates a Module class, which is defined in module.h
The module class can be constructed from a file which contains a verilog module definition.
The module class can then create an instance or a testbench of said module.
The testbench was the original intent of the project, but now it is being superseded by the instantiator.
Features which are still being worked on:
    - Parameters
    - Parameterized port widths
    - Procedurally generated comments, similar to xilinx IP .veo files

template2.txt is currently being used as the template for the instantiator.

template.txt contains plans for a more robust, almost DSL like template system. This will probably never be implemented due to a lack of demand and resources, and being limited in capabilities.

The intended use for this program is with notepad++ and the plugin pork2sausage.
Said plugin allows for selected text to be put into a file, then for text to be read from another file, and pasted into the selection.
Therefore, this program, once complete, will allow for the user to select a module definition, run the program, and have the instantiator generate an instance of the module in the file.

Note: I recognize c++ is not the best language for this, but this code is from when I only new C/C++, and it was easier to expand the bad code than rewrite it in another language, as the scope of this project is very limited.
Should the scope be expanded, another language should be chosen to parse text.