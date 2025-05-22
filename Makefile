#--------------------------------------------------------------------

CXX_DEBUG_FLAGS ?= -D_DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations           \
                   -Wc++14-compat -Wmissing-declarations -Wdeprecated -Wcast-align -Wcast-qual -Wchar-subscripts   \
                   -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal           \
                   -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op      \
                   -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self      \
                   -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel                \
                   -Wstrict-overflow=2 -Wsuggest-final-methods -Wsuggest-final-types  							   \
                   -Wsuggest-override -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code                      \
                   -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers   \
                   -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new                   \
                   -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging                 \
                   -fno-omit-frame-pointer -Werror=vla                                                             \
                   -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# -pie -fPIE
#--------------------------------------------------------------------

BUILD_DIR ?= build
LIB_DIR ?= lib

FRONTEND_EXECUTABLE_BINARY = $(BUILD_DIR)/frontend.x
BACKEND_EXECUTABLE_BINARY  = $(BUILD_DIR)/backend.x

FRONTEND_DIR = frontend
BACKEND_DIR  = backend
MODULES_DIR  = modules

FRONTEND_INCLUDE_DIR = $(FRONTEND_DIR)/include
BACKEND_INCLUDE_DIR  = $(BACKEND_DIR)/include
MODULES_INCLUDE_DIR  = $(MODULES_DIR)/include

FRONTEND_SRC_DIR = $(FRONTEND_DIR)/src
BACKEND_SRC_DIR  = $(BACKEND_DIR)/src
MODULES_SRC_DIR  = $(MODULES_DIR)/src

FRONTEND_SRC = $(FRONTEND_SRC_DIR)/main.c $(FRONTEND_SRC_DIR)/tokenization.c $(FRONTEND_SRC_DIR)/parse.c
BACKEND_SRC  = $(BACKEND_SRC_DIR)/main.c $(BACKEND_SRC_DIR)/ir.c $(BACKEND_SRC_DIR)/ir-generate.c $(BACKEND_SRC_DIR)/ir-parse.c
MODULES_SRC  = $(MODULES_SRC_DIR)/ast.c $(MODULES_SRC_DIR)/token.c $(MODULES_SRC_DIR)/common.c

FRONTEND_OBJ = $(addprefix $(BUILD_DIR)/, $(FRONTEND_SRC:.c=.o))
BACKEND_OBJ  = $(addprefix $(BUILD_DIR)/, $(BACKEND_SRC:.c=.o))
MODULES_OBJ  = $(addprefix $(BUILD_DIR)/, $(MODULES_SRC:.c=.o))

OBJ = $(FRONTEND_OBJ) $(BACKEND_OBJ) $(MODULES_OBJ)

#--------------------------------------------------------------------

INCLUDES = -I$(FRONTEND_INCLUDE_DIR) -I$(BACKEND_INCLUDE_DIR) -I$(MODULES_INCLUDE_DIR)
LIST_LIB = -llist

override CXXFLAGS += $(INCLUDES)

ifneq (0, $(words $(findstring -DDEBUG, $(CXXFLAGS))))
override CXXFLAGS += $(CXX_DEBUG_FLAGS)
override LDFLAGS  += $(CXX_DEBUG_FLAGS)
else
override CXXFLAGS += -DNDEBUG
endif

#--------------------------------------------------------------------

.PHONY: all
all: $(FRONTEND_EXECUTABLE_BINARY) $(BACKEND_EXECUTABLE_BINARY)

# Frontend executable
$(FRONTEND_EXECUTABLE_BINARY): $(FRONTEND_OBJ) $(MODULES_OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@

# Backend executable
$(BACKEND_EXECUTABLE_BINARY): $(BACKEND_OBJ) $(MODULES_OBJ)
	$(CXX) $(LDFLAGS) $^ -L$(LIB_DIR) $(LIST_LIB) -o $@

# Static pattern rule for compiling .c to .o
$(OBJ): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

#--------------------------------------------------------------------

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

#--------------------------------------------------------------------
