Spend a few hours coding a solution that would have only cost about 20 minutes to do manually. This is the way.

This program reads in a JSON schema into stdin and generates C++ header file. I still need to tackle enums, but for large JSON schemas this will still save time. If nothing else this provides a good introduction to bison, flex, grammar, and abstract syntax trees. 

`make
./jsonSchema < test-schemas/example7-schema.json `

