### Generated by rprotoc. DO NOT EDIT!
### <proto file: test/proto/lowercase.proto>
# package test.lower_case.lowerCamelCase;
# 
# message foo {  
#   message bar {
#     message fooBar {
#     }
#   } 
# }
# message baaz {  
#   required foo.bar x = 1;
# }
# enum baaaz {
#   abc = 0;
#   def = 1;
# }
# 
# service qux {
#   rpc quuuux(baaz) returns (foo);
# }

require 'protobuf/message/message'
require 'protobuf/message/enum'
require 'protobuf/message/service'
require 'protobuf/message/extend'

module Test
  module LowerCase
    module LowerCamelCase
      class Foo < ::Protobuf::Message
        defined_in __FILE__
        class Bar < ::Protobuf::Message
          defined_in __FILE__
          class FooBar < ::Protobuf::Message
            defined_in __FILE__
          end
        end
      end
      class Baaz < ::Protobuf::Message
        defined_in __FILE__
        required :'foo::bar', :x, 1
      end
      class Baaaz < ::Protobuf::Enum
        defined_in __FILE__
        define :abc, 0
        define :def, 1
      end
    end
  end
end