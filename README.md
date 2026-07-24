Spend a few hours coding a solution that would have only cost about 20 minutes to do manually. This is the way.

This program reads in a JSON schema into stdin and generates C++ header file. Maybe one day I'll get to generating the cpp files as well.

`make`
`./jsonSchema < test-schemas/example7-schema.json `

The benifit of building this from the Abstract Syntax Tree, as opposed to throwing everything in a map like similar programs do, is that the order is maintained from the JSON file to the C++ struct. This meant it was easier to go from a JSON spec to, say, a packet layout. 
This other benifit of this code is that, if maximums and minimuns are set, this program will choose the smallest integer type, as opposed to always selecting int64_t for JSON "integer" type and double for JSON "number" type.

