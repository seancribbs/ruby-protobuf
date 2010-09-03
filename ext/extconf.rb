require 'mkmf'

have_header("ruby/st.h")

create_makefile('cprotobuf')
