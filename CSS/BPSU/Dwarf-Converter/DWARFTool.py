'''
Created on 2017. nov. 8.

@author: z003s2dr

This module extracts information from the DWARF section of an ELF/COFF file regarding global variables.
This includes: name, type, location. Composite variables are also supported (struct, array, union)
The Variable class also gives means to convesions between binary and Python native representaions
'''

import logging

from elftools.common.py3compat import bytes2str
from elftools.elf.elffile import ELFFile
from elftools.dwarf.dwarfinfo import DebugSectionDescriptor, DWARFInfo, DwarfConfig
if __name__ == "DWARFTool" or __name__ == "__main__":
    from ticoff import Coff
else:
    from .ticoff import Coff
import struct
import copy
from io import BytesIO
import re


logger = logging.getLogger(__name__)


class Variable():
    """
    Contains all the attributes of a global variable extracted from the ELF file
    """

    def __init__(self, name="", location=0, fmt="", byteSize=0, encoding="", members=None,
                 varType="", cType="", typedef="", isLittleEndian=True, bitsInByte=8):
        self.name = name
        self.location = location
        self.fmt = fmt  # used only with a base variable
        self.byteSize = byteSize
        if members != None:
            self.members = members
        else:
            self.members = []
        self.encoding = encoding
        self.varType = varType
        self.cType = cType
        self.typedef = typedef
        self.bitMask = None  # if set to an integer, the variable is a bitfield

        # additional params
        self.enumDict = {}
        self.isPointer = False
        self.parent = None
        self.isComplete = False
        self.isLittleEndian = isLittleEndian
        # on some architectures, (for example, C2000, byte is 16 bits)
        self.bitsInByte = bitsInByte

    # known variable types
    varTypes = [
        "base",
        "array",
        "struct"]

    def __str__(self):
        if "array" == self.varType:
            if len(self.members) > 0:
                return ("%s Variable: %s, Location: 0x%X, Base: %s, Typedef: %s, length: %d"
                        % (self.varType, self.name, self.location, str(self.members[0]), self.typedef, len(self.members)))
            else:
                return ("%s Variable: %s, Location: 0x%X, Base: %s, Typedef: %s, length: %d"
                        % (self.varType, self.name, self.location, "??????", self.typedef, len(self.members)))
            
        elif "struct" == self.varType:
            membersDesc = ""
            for member in self.members:
                membersDesc += "%s\n" % member
            return "%s Variable: %s, Typedef: %s, Location: 0x%X, Members:\n" % (self.varType, self.name, self.typedef, self.location) + membersDesc

        else:
            return ("%s Variable: %s, Location: 0x%X, Encoding: %s, byteSize: %s, Typedef: %s, cType: %s"
                    % (self.varType, self.name, self.location, self.encoding, self.byteSize, self.typedef, self.cType))

    def addMember(self, newMember):
        self.members.append(newMember)
        newMember.parent = self

    def determineFormatString(self):
        # determines the format string for the python Struct module
        # TODO: this will not work on C2000 due to 16-bit byte-size

        fmt = ""
        if "DW_ATE_address" == self.encoding and 4 == self.byteSize:
            fmt = "L"
        elif "DW_ATE_float" == self.encoding:
            if 4 == self.byteSize:
                fmt = "f"
            elif 8 == self.byteSize:
                fmt = "d"
        elif "DW_ATE_signed" == self.encoding:
            if 2 == self.byteSize:
                fmt = "h"
            elif 4 == self.byteSize:
                fmt = "l"
            elif 8 == self.byteSize:
                fmt = "q"
        elif "DW_ATE_unsigned" == self.encoding:
            if 2 == self.byteSize:
                fmt = "H"
            elif 4 == self.byteSize:
                fmt = "L"
            elif 8 == self.byteSize:
                fmt = "Q"
        elif "DW_ATE_signed_char" == self.encoding:
            fmt = "b"
        elif "DW_ATE_unsigned_char" == self.encoding:
            fmt = "B"

        if "" != fmt:  # add endiannes specifier is format is valid
            if self.isLittleEndian:
                fmt = "<" + fmt
            else:
                fmt = ">" + fmt
        self.fmt = fmt

    def Finish(self):
        """
        Fills some other member variables for convenience based on the collected data
        Validate if variable is completely parsed
        """
        if self.isPointer:  # todo
            self.typedef = "Pointer"
            self.isComplete = True
        if "" == self.typedef:
            self.typedef = self.cType

        if "array" == self.varType:
            self.isComplete = True

        elif "struct" == self.varType:
            self.typedef = "struct " + self.typedef
            self.isComplete = True

        elif "union" == self.varType:
            self.typedef = "union " + self.typedef
            self.isComplete = True

        elif "base" == self.varType:
            self.determineFormatString()
            self.isComplete = True

    def decode(self, rawBytes, decodeFormat="str"):
        """
        Returns an object representing the value decoded.
        decodeFormat:
        "str": string representation, including decoded ENUM
        "float": float value
        "enum": only the enum, None if No associated enum value exists 
        """
        # TODO: this will not work on C2000 due to 16-bit byte-size
        rawBytes = bytes(rawBytes[0: self.byteSize])
        retVal = None
        decodedEnum = None
        try:
            unpakt = struct.unpack(self.fmt, rawBytes)[0]
            try:
                decodedEnum = self.enumDict[unpakt]
            except KeyError:
                pass
        except Exception as e:
            logger.error("Error decoding data for %s \n %s" % (self, e))
            return retVal

        if("str" == decodeFormat) and decodedEnum is not None:
            retVal = "%s (%s)" % (unpakt, decodedEnum)
        elif("str" == decodeFormat):
            retVal = str(unpakt)
        elif("float" == decodeFormat):
            retVal = float(unpakt)
        elif("enum" == decodeFormat):
            retVal = decodedEnum

        return retVal

    def encode(self, value):
        """
        Try to encode to raw bytes according to type. Only works with base variable.
        Value should be a string normally.
        Returns empty bytearray if unsuccessful.
        """
        # TODO: this will not work on C2000 due to 16-bit byte-size
        try:
            if "DW_ATE_float" == self.encoding:
                value = float(value)
            else:
                if 0 == value.find("0x"):
                    value = int(value, 16)
                else:
                    value = int(value)
        except:
            pass

        retVal = bytes()
        if "base" == self.varType:
            try:
                retVal = struct.pack(self.fmt, value)
            except struct.error as e:
                logger.error("Cannot encode %s, %s" % (value, e))
        else:
            logger.error("Cannot encode %s" % self)

        return retVal


class DwarfExtractor():
    DW_ATE_Table = {
        1: "DW_ATE_address",
        2: "DW_ATE_boolean",
        3: "DW_ATE_complex_float",
        4: "DW_ATE_float",
        5: "DW_ATE_signed",
        6: "DW_ATE_signed_char",
        7: "DW_ATE_unsigned",
        8: "DW_ATE_unsigned_char",
        0x80: "DW_ATE_lo_user",
        0xfe: "enum_hotfix",
        0xff: "DW_ATE_hi_user"
    }

    def __init__(self):
        self.variables = {}
        self.dieIndex = {}
        self.little_endian = True
        self.elffile = None
        self.dwarfinfo = None
        self.bitsInByte = 8

    def process_coff_file(self, filename, skipRegex):
        coffObj = Coff(filename)

        # FIX for pyelftools v0.29:
        # Had to add debug_pubtypes and the rest
        section_names = ('.debug_info', '.debug_aranges', '.debug_abbrev', '.debug_frame',
                         '.eh_frame', '.debug_str', '.debug_loc', '.debug_ranges', '.debug_line',
                         '.debug_pubtypes', '.debug_pubnames', '.debug_addr', '.debug_str_offsets',
                         '.debug_line_str', '.debug_loclists', '.debug_rnglists', '.debug_sup', '.gnu_debugaltlink')

        debug_sections = []
        # todo: check endianness in coffObj.header.flags

        # C2000 byte size is 16 bits, what about the others?
        if coffObj.header.target_id == "TMS320C2800":
            self.bitsInByte = 16

        for secname in section_names:
            section = coffObj.get_section_by_name(secname)
            if section is not None:
                sectionDesc = DebugSectionDescriptor(
                    stream=BytesIO(section.data),
                    name=secname,
                    global_offset=section.raw_data_ptr,
                    size=section.raw_data_size,
                    address=0)  # "'address' is required to properly decode the special '.eh_frame' format" <-- we don't need eh_frame but the argument is required
                debug_sections.append(sectionDesc)
            else:
                debug_sections.append(None)



        cfg = DwarfConfig(little_endian=True,
                          default_address_size=4, machine_arch="")
        self.dwarfinfo = DWARFInfo(cfg, *debug_sections)
        self.process_Dwarfinfo(skipRegex)

    def process_elf_file(self, filename, skipRegex):
        logger.info('Processing file: %s' % filename)
        with open(filename, 'rb') as f:
            self.elffile = ELFFile(f)
            self.little_endian = self.elffile.little_endian
            # TODO: C2000 byte size is 16 bits, what about the others?
            if self.elffile.header.e_machine == "EM_TI_C2000":
                self.bitsInByte = 16

            if not self.elffile.has_dwarf_info():
                logger.error('  file has no DWARF info')
                return

            # get_dwarf_info returns a DWARFInfo context object, which is the
            # starting point for all DWARF-based processing in pyelftools.
            self.dwarfinfo = self.elffile.get_dwarf_info()
            self.process_Dwarfinfo(skipRegex)

    def process_Dwarfinfo(self, skipRegex):
        self.indexDwarf()

        # iterate over compile units
        for CU in self.dwarfinfo.iter_CUs():
            logger.debug('\n\n  Found a compile unit at offset %s, length %s' % (
                CU.cu_offset, CU['unit_length']))

            # Start with the top DIE, the root for this CU's DIE tree
            top_DIE = CU.get_top_DIE()
            logger.debug('    Top DIE with tag=%s' % top_DIE.tag)

            if 'DW_AT_name' in top_DIE.attributes:
                name_attr = top_DIE.attributes['DW_AT_name']
                logger.debug('    name=%s' % bytes2str(name_attr.value))

                # create the variables list
                try:
                    self.getVarsFromTopDie(top_DIE, skipRegex)
                except KeyError as e:
                    logger.error(str(e))
            else:
                logger.debug("Top DIE has no name")
        logger.info("Got %d global variables" % len(self.variables))

    def indexDwarf(self):
        """
        Iterates over all children of all CUs in the dwarf section and indexes them in a dict with their offsets (relative to the start of the dwarf setction)
        """
        for CU in self.dwarfinfo.iter_CUs():
            topDIE = CU.get_top_DIE()
            for subDie in topDIE.iter_children():
                self.dieIndex[subDie.offset] = subDie

    def getReferencedDie(self, cu, refAttr):
        """
        Get the referenced DIE from the index. refAttr is usually a DW_AT_Type or DW_AT_specification
        two types of references are supported.
        """
        if refAttr.form == "DW_FORM_ref_addr":
            # absolute address from the beginnig of the dwarf section
            return self.dieIndex[refAttr.value]
        elif refAttr.form in ["DW_FORM_ref1", "DW_FORM_ref2", "DW_FORM_ref4", "DW_FORM_ref8", "DW_FORM_ref_udata"]:
            # relative address from the beginnig of CU
            return self.dieIndex[refAttr.value + cu.cu_offset]
        else:
            logger.warn("Unknown reference form: %s" % str(refAttr))

    def createArrayMembersRecursive(self, remainingArrayDims, baseVar, parent):
        """
        Creates the tree structure of Variables for multidimensional arrays
        TODO: check dimension priorities?
        """
        if len(remainingArrayDims) > 1:
            # create array variable to act as a parent for sub arrays
            for i in range(0, remainingArrayDims[0]):
                subarraySize = 1
                typedefTxt = baseVar.typedef
                for d in remainingArrayDims[1:]:
                    subarraySize *= d
                    typedefTxt += "[%d]" % d
                subarraySize *= baseVar.byteSize
                subarray = Variable(name="%s[%d]" % (parent.name, i),
                                    varType="array",
                                    typedef=typedefTxt,
                                    isLittleEndian=self.little_endian,
                                    location=parent.location,
                                    byteSize=subarraySize)
                parent.addMember(subarray)
                self.createArrayMembersRecursive(
                    remainingArrayDims[1:], baseVar, subarray)
                self.setArrayMemberLocationOffsetsRecursive(
                    subarray, subarray.byteSize * i)
        else:
            # add baseVar members
            for i in range(0, remainingArrayDims[0]):
                newMember = copy.deepcopy(baseVar)
                newMember.name = "%s[%d]" % (parent.name, i)
                parent.addMember(newMember)
                self.setArrayMemberLocationOffsetsRecursive(
                    newMember, newMember.byteSize * i)

    def trackVariableTypeRecursive(self, die, variable, reclevel=0):
        """
        Walk through the reference chain of the variable's 'DW_AT_type' attribute and children
        to collect information about the variable's type, struct members, etc
        """
        logger.debug("[%s] Tracking var Type. die.tag: %s, offset: 0x%X" % (
            reclevel, die.tag, die.offset))

        if "DW_TAG_base_type" == die.tag:
            # arrived at base type, end of reference chain
            variable.encoding = self.DW_ATE_Table[die.attributes["DW_AT_encoding"].value]
            variable.cType = die.attributes["DW_AT_name"].value.decode()
            variable.byteSize = die.attributes["DW_AT_byte_size"].value
            variable.varType = "base"
            logger.debug("Parsed: %s" % str(variable))

        elif "DW_TAG_array_type" == die.tag:
            variable.varType = "array"

            # determine base variable for array
            tdDieOffset = die.attributes["DW_AT_type"]
            tdDie = self.getReferencedDie(die.cu, tdDieOffset)
            arrayBaseVar = Variable(
                location=variable.location, isLittleEndian=self.little_endian)
            self.trackVariableTypeRecursive(
                tdDie, arrayBaseVar, reclevel=reclevel + 1)

            # determine length and dimensions of array
            # todo: check for array ordering?    DW_ORD_col_major or DW_ORD_col_minor
            arrayDims = []
            arrayLen = 1
            for subrangedie in die.iter_children():
                if "DW_TAG_subrange_type" == subrangedie.tag and "DW_AT_upper_bound" in subrangedie.attributes:
                    dim = subrangedie.attributes["DW_AT_upper_bound"].value + 1
                    arrayDims.append(dim)
                    arrayLen *= dim
                else:
                    logger.warn(
                        "Could not determine array length for variable %s, DW_AT_upper_bound not present." % variable.name)
                    return

            if 0 != arrayBaseVar.byteSize:
                variable.byteSize = arrayLen * arrayBaseVar.byteSize
                self.createArrayMembersRecursive(
                    arrayDims, arrayBaseVar, variable)
                typedefTxt = arrayBaseVar.typedef
                for dim in arrayDims:
                    typedefTxt += "[%d]" % dim
                variable.typedef = typedefTxt
                logger.debug("Parsed. %s" % str(variable))

            else:
                logger.warn(
                    "Could not determine array length for variable %s, base type has no valid byteSize" % variable.name)

            variable.Finish()
            return

        elif "DW_TAG_enumeration_type" == die.tag:
            for subDie in die.iter_children():
                if "DW_TAG_enumerator" == subDie.tag:
                    enumname = subDie.attributes["DW_AT_name"].value.decode()
                    enumval = subDie.attributes["DW_AT_const_value"].value
                    variable.enumDict[enumval] = enumname
            # FIXME: TODO: We just handle enumeration as a base integer type here
            # to support HiTerm to read enums as simple integers
            variable.encoding = self.DW_ATE_Table[0xfe]
            variable.cType = ''
            variable.byteSize = die.attributes["DW_AT_byte_size"].value
            variable.varType = "base"
            logger.debug("Parsed: %s" % str(variable))
            logger.debug("Enum values: %s" % variable.enumDict)

        elif "DW_TAG_structure_type" == die.tag:
            variable.varType = "struct"
            variable.byteSize = die.attributes["DW_AT_byte_size"].value
            for idx, subDie in enumerate(die.iter_children()):
                if "DW_TAG_member" == subDie.tag:
                    # get member location
                    locAattr = subDie.attributes["DW_AT_data_member_location"]
                    if locAattr.form in ["DW_FORM_data1", "DW_FORM_data2", "DW_FORM_data4", "DW_FORM_data8"]:
                        memberLocation = variable.location + locAattr.value
                    elif locAattr.form == "DW_FORM_block1":
                        # TODO DW_FORM_block1 representation of member location is not quite clear yet. [35, 154, 1] represents DW_OP_plus_uconst 154 in objdump...
                        # FIXME: [35, 154, 1] seems to represent 1*128 + (154-128) offset instead and [35, 127] represents 127 offset
                        # For now only 2 and 3 length is implemented, probably when there is a quite long object we will have more length
                        # In that case we will fire a warning message
                        if len(locAattr.value) == 2:
                            memberLocation = variable.location + \
                                int(locAattr.value[1])
                        elif len(locAattr.value) == 3:
                            memberLocation = variable.location + \
                                int(locAattr.value[1]) - 128 + \
                                int(locAattr.value[2]) * 128
                        else:
                            logger.warn(
                                "Could not determine data member location!")
                    elif locAattr.form == "DW_FORM_block":
                        # FIXME: lets assume that this the same as DW_FORM_block1
                        if len(locAattr.value) == 2:
                            memberLocation = variable.location + \
                                int(locAattr.value[1])
                        elif len(locAattr.value) == 3:
                            memberLocation = variable.location + \
                                int(locAattr.value[1]) - 128 + \
                                int(locAattr.value[2]) * 128
                        else:
                            logger.warn(
                                "Could not determine data member location!")
                    else:
                        logger.warn(
                            "Data member offset form not supported! attr: %s" % str(locAattr))
                        break
                    memberName = subDie.attributes["DW_AT_name"].value.decode()
                    member = Variable(
                        name=memberName, location=memberLocation, isLittleEndian=self.little_endian)
                    logger.debug("[%d] Tracking member [%d]: %s" %
                                 (reclevel, idx, memberName))
                    ref = subDie.attributes["DW_AT_type"]
                    tdDie = self.getReferencedDie(die.cu, ref)
                    self.trackVariableTypeRecursive(
                        tdDie, member, reclevel=reclevel + 1)
                    isSuccess = True
                    if "DW_AT_bit_offset" in subDie.attributes and "DW_AT_bit_size" in subDie.attributes:
                        # create bit mask for the bitfield. DWARF 4 and 3 store these values differently
                        bitsize = subDie.attributes["DW_AT_bit_size"].value
                        bitoffs = subDie.attributes["DW_AT_bit_offset"].value
                        if die.cu.header.version == 4 and self.little_endian:
                            member.bitMask = ((1 << bitsize) - 1) << bitoffs
                        else:  # version 3 big endian and version 4
                            try:
                                member.bitMask = (
                                    (1 << bitsize) - 1) << (member.byteSize * self.bitsInByte - bitsize - bitoffs)
                            except ValueError:
                                logger.warn("Negative shift count encountered while calculating bitmask at struct:%s, member:%s" % (
                                    variable.name, member.name))
                                isSuccess = False
                                # This might be caused by incorrectly defining shorter data type for a bitfield struct member
                        if member.bitMask is not None:
                            logger.debug("[%d] Set bitMask: 0x%X" %
                                         (reclevel, member.bitMask))
                    if isSuccess:
                        variable.addMember(member)
                else:
                    logger.error(
                        "Structure type's child is not a DW_TAG_member: %s" % subDie)
                    break
            logger.debug("Parsed %s" % variable)
            variable.Finish()
            return

        elif "DW_TAG_typedef" == die.tag:
            if "" == variable.typedef:
                variable.typedef = die.attributes["DW_AT_name"].value.decode()

        elif "DW_TAG_pointer_type" == die.tag:
            variable.isPointer = True
            # this is just an assumption, may not work on all architectures...
            variable.byteSize = 32 // self.bitsInByte
            # todo handle pointed data types..?
            variable.Finish()
            return
        elif "DW_TAG_union_type" == die.tag:
            variable.varType = "union"
            variable.byteSize = die.attributes["DW_AT_byte_size"].value
            for idx, subDie in enumerate(die.iter_children()):
                if "DW_TAG_member" == subDie.tag:
                    # get member location
                    locAattr = subDie.attributes["DW_AT_data_member_location"]
                    if locAattr.form in ["DW_FORM_data1", "DW_FORM_data2", "DW_FORM_data4", "DW_FORM_data8"]:
                        memberLocation = locAattr.value
                    elif locAattr.form == "DW_FORM_block1":
                        memberLocation = variable.location + \
                            int(locAattr.value[1])
                    elif locAattr.form == "DW_FORM_block":
                        # FIXME: lets assume that this the same as DW_FORM_block1
                        memberLocation = variable.location + \
                            int(locAattr.value[1])
                    else:
                        logger.warn(
                            "Data member offset form not supported! attr: %s" % str(locAattr))
                        break
                    memberName = subDie.attributes["DW_AT_name"].value.decode()
                    member = Variable(
                        name=memberName, location=memberLocation, isLittleEndian=self.little_endian)
                    ref = subDie.attributes["DW_AT_type"]
                    tdDie = self.getReferencedDie(die.cu, ref)
                    logger.debug("[%d] Tracking member [%d]: %s" %
                                 (reclevel, idx, memberName))
                    self.trackVariableTypeRecursive(
                        tdDie, member, reclevel=reclevel + 1)
                    variable.addMember(member)
                else:
                    logger.error(
                        "Structure type's child is not a DW_TAG_member: %s" % subDie)
                    break
            logger.debug("Parsed %s" % variable)
            variable.Finish()
            return

        # proceed further with type description chain if available
        tdDie = None
        try:
            ref = die.attributes["DW_AT_type"]
            tdDie = self.getReferencedDie(die.cu, ref)
        except KeyError:
            pass
        if None is not tdDie:
            self.trackVariableTypeRecursive(
                tdDie, variable, reclevel=reclevel + 1)
        else:
            variable.Finish()

    def getVarsFromTopDie(self, topDie, skipRegex):
        for subDie in topDie.iter_children():
            if subDie.tag == "DW_TAG_variable" and "DW_AT_location" in subDie.attributes:
                location = subDie.attributes["DW_AT_location"].value
                if self.little_endian:
                    location = struct.unpack("<xI", bytes(location))[0]
                else:
                    location = struct.unpack(">xI", bytes(location))[0]
                var = Variable(location=location,
                               isLittleEndian=self.little_endian)
                if "DW_AT_specification" in subDie.attributes:
                    # this is the definition of a non-defining declaration, jump to the definition
                    logger.debug("DW_TAG_variable, non defining declaration")
                    ref = subDie.attributes["DW_AT_specification"]
                    try:
                        defDie = self.getReferencedDie(topDie.cu, ref)
                    except KeyError:
                        logger.warn(
                            "Could not find referred DIE in CU's index...")
                        continue
                    self.collectVarDefinition(defDie, var, skipRegex)
                elif "DW_AT_name" in subDie.attributes:
                    self.collectVarDefinition(subDie, var, skipRegex)

                if var.isComplete:
                    # var is incomplete if it is already in self.variables (see collectVarDefinition), or there was an error during parsing
                    self.variables[var.name] = var

    def setArrayMemberLocationOffsetsRecursive(self, arrayMember, offset):
        """
        The location of array members have to be manually offset due to copying
        """
        arrayMember.location += offset

        # if there are submembers, set those locations too
        for member in arrayMember.members:
            self.setArrayMemberLocationOffsetsRecursive(member, offset)

    def collectVarDefinition(self, definingDIE, varInstance, skipRegex):
        """
        Get variable name and type description
        """
        varInstance.name = definingDIE.attributes["DW_AT_name"].value.decode()
        if skipRegex is not None:
            if re.match(skipRegex, varInstance.name) is not None:
                logger.info("Regex match. Skipping %s" % (varInstance.name))
                return
        # do not proces variable if already processed
        if varInstance.name not in self.variables:
            logger.debug("DW_TAG_variable, definition. Name: %s" %
                         (varInstance.name))
            # do not process already seen variable again
            self.trackVariableTypeRecursive(definingDIE, varInstance)
            # self.setArrayMemberLocationsRecursive(varInstance)


if __name__ == '__main__':
    logger.setLevel(logging.DEBUG)
    # create console handler and set level to debug
    ch = logging.StreamHandler()
    ch.setLevel(logging.WARNING)
    # create file handler which logs even debug messages
    fh = logging.FileHandler('DWARFTool.log')
    fh.setLevel(logging.DEBUG)
    # create formatter
    formatter = logging.Formatter(
        '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    formatter = logging.Formatter('%(name)s - %(levelname)s - %(message)s')
    # add formatter to ch
    ch.setFormatter(formatter)
    fh.setFormatter(formatter)
    # add ch to logger
    logger.addHandler(ch)
    logger.addHandler(fh)
    e = DwarfExtractor()  
    # e.process_elf_file("VCU_V2.out")
    e.process_coff_file("powerboard_app_manual.out")
