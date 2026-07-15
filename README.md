Spend a few hours coding a solution that would have only cost about 20 minutes to do manually. This is the way.

This program reads in a JSON schema into stdin and generates C++ header file. I still need to tackle enums, but for large JSON schemas this will still save time. If nothing else this provides a good introduction to bison, flex, grammar, and abstract syntax trees. 

`make`
`./jsonSchema < test-schemas/example7-schema.json `

The benifit of building this from the Abstract Syntax Tree, as opposed to throwing everything in a map like similar programs do, is that the order is maintained from the JSON file to the C++ struct. This meant it was easier to go from a JSON spec to, say, a packet layout.

