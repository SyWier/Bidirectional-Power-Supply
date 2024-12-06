import argparse
import sys
import logging
import DWARFTool
import re


class sorter():

    def __init__(self):
        self.inputFile = ""
        self.inputVars = dict()

    def asciiSorting(self, l):
        def convert(text):
            return int(text) if text.isdigit() else text

        def asciiKey(key):
            # This was removed because HiTerm was not working properly with this sorting
            # It seems that the [] characters are earlier in HiTerm than numbers, which
            # is not correct according to the ASCII table. 
            # Python sorted() function is using ASCII sorting.
            # A walkaround was to replace each [] characters with (), which has lower
            # ASCII code. 
            # With multi dimensional arrays it is still buggy.
            # return [convert(c)
                    # for c in re.split('([0-9]+)', key)]
            to_ret = str(key).replace('[','(').replace(']',')')
            return to_ret

        return sorted(l, key=asciiKey)

    def getFirstWord(self, fullLine):
        word = fullLine.split()
        if 4 == len(word):
            return word[0]

    def createDictItemFromVarDefLine(self, fullLine):
        word = fullLine.split()
        # Check that it is a real variable definition.
        # Dummy check just see that it has for columns, but do not check the content.
        if 4 != len(word):
            return
        # Get the variable name.
        var = word[0]
        # Get the variable params.
        return {var.lower(): fullLine}

    def createFileWithSortedVars(self, inputFile, outFile, isAscii):
        inputFile = open(inputFile, "r")

        for line in inputFile:
            self.inputVars.update(self.createDictItemFromVarDefLine(line))

        outFile = open(outFile, "w")
        # ASCII sorting seems to provide better results
        if isAscii:
            for key in iter(self.asciiSorting(self.inputVars)):
                outFile.write(self.inputVars[key])
        else:
            for key in sorted(iter(self.inputVars)):
                outFile.write(self.inputVars[key])


class converter():
    # dict where the key is the (var.encoding, var.byteSize) tuple for 8-bit units
    typeSpecs8bit = {
        ("DW_ATE_float", 4): "%FLOAT #Signed",
        ("DW_ATE_float", 8): "%DOUBLE #Signed",
        ("DW_ATE_signed", 2): "%S16 #Signed",
        ("DW_ATE_signed", 4): "%S32 #Signed",
        ("DW_ATE_signed", 8): "%S64 #Signed",
        ("DW_ATE_unsigned", 2): "%U16 #Unsigned",
        ("DW_ATE_unsigned", 4): "%U32 #Unsigned",
        ("DW_ATE_unsigned", 8): "%U64 #Unsigned",
        ("DW_ATE_signed_char", 1): "%S8 #Signed",
        ("DW_ATE_unsigned_char", 1): "%UCHAR #Unsigned"
    }
    # dict where the key is the (var.encoding, var.byteSize) tuple for 16-bit units
    typeSpecs16bit = {
        ("DW_ATE_float", 2): "%FLOAT #Signed",
        ("DW_ATE_float", 4): "%DOUBLE #Signed",
        ("DW_ATE_signed", 1): "%S16 #Signed",
        ("DW_ATE_signed", 2): "%S32 #Signed",
        ("DW_ATE_signed", 4): "%S64 #Signed",
        ("DW_ATE_unsigned", 1): "%U16 #Unsigned",
        ("DW_ATE_unsigned", 2): "%U32 #Unsigned",
        ("DW_ATE_unsigned", 4): "%U64 #Unsigned",
        ("DW_ATE_signed_char", 1): "%S16 #Signed",
        ("DW_ATE_unsigned_char", 1): "%U16 #Unsigned",
        ("enum_hotfix", 1): "%U16 #Unsigned",
        ("enum_hotfix", 2): "%U32 #Unsigned",
        ("enum_hotfix", 4): "%U64 #Unsigned",
    }
    # use 8bits by default. the run method will overwrite this to the requested one.
    typeSpecs = typeSpecs8bit

    def __init__(self):
        self.logger = logging.getLogger(__name__)
        self.ch = logging.StreamHandler()
        self.ch.setLevel(logging.DEBUG)
        self.fh = logging.FileHandler("dwarformer.log", mode='w')
        self.fh.setLevel(logging.DEBUG)
        # create formatter
        self.formatter = logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        # add formatter to ch
        self.ch.setFormatter(self.formatter)
        self.fh.setFormatter(self.formatter)
        # add ch to logger
        self.logger.addHandler(self.ch)
        self.logger.addHandler(self.fh)

        DWARFTool.logger.setLevel(logging.DEBUG)
        DWARFTool.logger.addHandler(self.ch)
        DWARFTool.logger.addHandler(self.fh)

    def setLoglevel(self, lvl):
        self.logger.setLevel(lvl)
        DWARFTool.logger.setLevel(lvl)

    def getTypeSpec(self, var):
        if var.bitMask is not None:
            return "%%B%d #MASK=0x%X" % (var.byteSize * 16, var.bitMask)
        else:
            return self.typeSpecs[(var.encoding, var.byteSize)]

    def findBaseVariablesRecursive(self, var, nameString=""):
        if var.varType == "base":
            try:
                typespec_str = self.getTypeSpec(var)
            except KeyError:
                self.logger.error("Unknown variable type encountered at %s: encoding: %s, byteSize:%s" % (
                    nameString + var.name, var.encoding, var.byteSize))
                return
            self.f.write("%s 0x%X %s\n" %
                         (nameString + var.name, var.location, typespec_str))

        else:
            for v in var.members:
                if var.varType != "array":
                    newNameString = nameString + var.name + "."
                else:
                    # FIX for 2+D arrays: in case of 2+D arrays the name will be an empty string in the infineon example elf file
                    if v.name.startswith("["):
                        newNameString = nameString + var.name
                    else:
                        newNameString = nameString
                self.findBaseVariablesRecursive(v, newNameString)

    def run(self, args):
        if int(args.bits) == 8:
            self.typeSpecs = self.typeSpecs8bit
        elif int(args.bits) == 16:
            self.typeSpecs = self.typeSpecs16bit
        if args.verbose:
            self.setLoglevel(logging.DEBUG)
        else:
            self.setLoglevel(logging.INFO)

        skipRegex = None
        if args.skipRegex is not None:
            skipRegex = re.compile(args.skipRegex)

        e = DWARFTool.DwarfExtractor()

        if args.elf == True:
            e.process_elf_file(args.inputFile, skipRegex)
        elif args.coff == True:
            e.process_coff_file(args.inputFile, skipRegex)
        

        with open(args.outFile, "w") as self.f:
            for v in e.variables.values():
                self.findBaseVariablesRecursive(v)

        if args.sortedOutFile:
            str = sorter()
            # use ASCII sorting by defeault
            isAscii = True
            str.createFileWithSortedVars(
                args.outFile, args.sortedOutFile, isAscii)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Extract variabled with '
                                                 'memory address from elf and coff file into text file.')
    parser.add_argument("-i", "--input", dest="inputFile", required=True,
                        help="Elf or coff file.")
    parser.add_argument("-o", "--out", dest="outFile", required=True,
                        help="This will be the txt output file.")
    parser.add_argument("-e", "--elf", action='store_true',
                        help="The input file is in elf format.")
    parser.add_argument("-c", "--coff", action='store_true',
                        help="The input file is in coff format.")
    parser.add_argument("-s", "--sort", dest="sortedOutFile",
                        help="The variables will be sorted in abc format in the output file.")
    parser.add_argument("-b", "--bits", dest="bits", required=True,
                        help="The data unit length in bits. BITS=8 or BITS=16")
    parser.add_argument("-v", "--verbose", action='store_true',
                        help="Enable verbose logging")
    parser.add_argument("-sr", "--skipregex", dest="skipRegex",
                        help="Skip processing variables which name matches the provided regular expression.")

    args = parser.parse_args()
    if args.elf == False and args.coff == False:
        print("Please define the format of input file with -e or -c.")
        parser.print_help()
    elif int(args.bits) != 8 and int(args.bits) != 16:
        print("Please define the data unit length with the -b option.")
        parser.print_help()
    else:
        conv = converter()
        conv.run(args)
