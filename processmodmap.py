#!/usr/bin/env python

import subprocess
import pprint
import json

charToName = {
 '0': '0',
 '1': '1',
 '2': '2',
 '3': '3',
 '4': '4',
 '5': '5',
 '6': '6',
 '7': '7',
 '8': '8',
 '9': '9',
 'A': 'A',
 'Alt_L': 'Alt_L',
 'B': 'B',
 'BackSpace': 'BackSpace',
 'Break': 'Break',
 'C': 'C',
 'Caps_Lock': 'Caps_Lock',
 'Control_L': 'Control_L',
 'Control_R': 'Control_R',
 'D': 'D',
 'Delete': 'Delete',
 'Down': 'Down',
 'E': 'E',
 'End': 'End',
 'Escape': 'Escape',
 'EuroSign': 'EuroSign',
 'F': 'F',
 'F1': 'F1',
 'F10': 'F10',
 'F11': 'F11',
 'F12': 'F12',
 'F2': 'F2',
 'F3': 'F3',
 'F4': 'F4',
 'F5': 'F5',
 'F6': 'F6',
 'F7': 'F7',
 'F8': 'F8',
 'F9': 'F9',
 'G': 'G',
 'H': 'H',
 'Home': 'Home',
 'Hyper_L': 'Hyper_L',
 'I': 'I',
 'ISO_Left_Tab': 'ISO_Left_Tab',
 'ISO_Level3_Shift': 'ISO_Level3_Shift',
 'Insert': 'Insert',
 'J': 'J',
 'K': 'K',
 'KP_0': 'KP_0',
 'KP_1': 'KP_1',
 'KP_2': 'KP_2',
 'KP_3': 'KP_3',
 'KP_4': 'KP_4',
 'KP_5': 'KP_5',
 'KP_6': 'KP_6',
 'KP_7': 'KP_7',
 'KP_8': 'KP_8',
 'KP_9': 'KP_9',
 'KP_Add': 'KP_Add',
 'KP_Begin': 'KP_Begin',
 'KP_Decimal': 'KP_Decimal',
 'KP_Delete': 'KP_Delete',
 'KP_Divide': 'KP_Divide',
 'KP_Down': 'KP_Down',
 'KP_End': 'KP_End',
 'KP_Enter': 'KP_Enter',
 'KP_Equal': 'KP_Equal',
 'KP_Home': 'KP_Home',
 'KP_Insert': 'KP_Insert',
 'KP_Left': 'KP_Left',
 'KP_Multiply': 'KP_Multiply',
 'KP_Next': 'KP_Next',
 'KP_Prior': 'KP_Prior',
 'KP_Right': 'KP_Right',
 'KP_Subtract': 'KP_Subtract',
 'KP_Up': 'KP_Up',
 'L': 'L',
 'Left': 'Left',
 'M': 'M',
 'Menu': 'Menu',
 'Meta_L': 'Meta_L',
 'Mode_switch': 'Mode_switch',
 'N': 'N',
 'Next': 'Next',
 'NoSymbol': 'NoSymbol',
 'Num_Lock': 'Num_Lock',
 'O': 'O',
 'P': 'P',
 'Pause': 'Pause',
 'Print': 'Print',
 'Prior': 'Prior',
 'Q': 'Q',
 'R': 'R',
 '\n': 'Return',
 'Right': 'Right',
 'S': 'S',
 'Scroll_Lock': 'Scroll_Lock',
 'Shift_L': 'Shift_L',
 'Shift_R': 'Shift_R',
 'Super_L': 'Super_L',
 'Super_R': 'Super_R',
 'Sys_Req': 'Sys_Req',
 'T': 'T',
 'Tab': 'Tab',
 'U': 'U',
 'Up': 'Up',
 'V': 'V',
 'W': 'W',
 'X': 'X',
 'Y': 'Y',
 'Z': 'Z',
 'a': 'a',
 'ae': 'ae',
 'à': 'agrave',
 '&': 'ampersand',
 "'": 'apostrophe',
 '^': 'asciicircum',
 '*': 'asterisk',
 '@': 'at',
 'b': 'b',
 '\\': 'backslash',
 '|': 'bar',
 '{': 'braceleft',
 '}': 'braceright',
 '[': 'bracketleft',
 ']': 'bracketright',
 'c': 'c',
 'ç': 'ccedilla',
 'cent': 'cent',
 ':': 'colon',
 ',': 'comma',
 'd': 'd',
 'dead_acute': 'dead_acute',
 'dead_cedilla': 'dead_cedilla',
 'dead_circumflex': 'dead_circumflex',
 'dead_diaeresis': 'dead_diaeresis',
 'dead_grave': 'dead_grave',
 'dead_hook': 'dead_hook',
 'dead_tilde': 'dead_tilde',
 '°': 'degree',
 '$': 'dollar',
 'downarrow': 'downarrow',
 'dstroke': 'dstroke',
 'e': 'e',
 'é': 'eacute',
 'è': 'egrave',
 'eng': 'eng',
 '=': 'equal',
 'eth': 'eth',
 '!': 'exclam',
 'f': 'f',
 'g': 'g',
 '>': 'greater',
 'guillemotleft': 'guillemotleft',
 'guillemotright': 'guillemotright',
 'h': 'h',
 'horizconnector': 'horizconnector',
 'hstroke': 'hstroke',
 'i': 'i',
 'j': 'j',
 'k': 'k',
 'kra': 'kra',
 'l': 'l',
 'leftarrow': 'leftarrow',
 'leftdoublequotemark': 'leftdoublequotemark',
 '<': 'less',
 'lstroke': 'lstroke',
 'm': 'm',
 '-': 'minus',
 'µ': 'mu',
 'n': 'n',
 'notsign': 'notsign',
 '#': 'numbersign',
 'o': 'o',
 'oe': 'oe',
 'onehalf': 'onehalf',
 'onequarter': 'onequarter',
 'p': 'p',
 '§': 'paragraph',
 '(': 'parenleft',
 ')': 'parenright',
 '%': 'percent',
 '.': 'period',
 'periodcentered': 'periodcentered',
 '+': 'plus',
 'q': 'q',
 '?': 'question',
 '"': 'quotedbl',
 'r': 'r',
 'rightarrow': 'rightarrow',
 'rightdoublequotemark': 'rightdoublequotemark',
 's': 's',
 'section': 'section',
 ';': 'semicolon',
 '/': 'slash',
 ' ': 'space',
 'ssharp': 'ssharp',
 '£': 'sterling',
 't': 't',
 'thorn': 'thorn',
 '³': 'threesuperior',
 'tslash': 'tslash',
 '²': 'twosuperior',
 'u': 'u',
 'ù': 'ugrave',
 '_': 'underscore',
 'v': 'v',
 'w': 'w',
 'x': 'x',
 'y': 'y',
 'z': 'z'
}

def getName(s):
    idx = s.find("(")
    if idx < 0:
        return ''
        #raise Exception(f"Can't find name in '{s}'")
    idx2 = s.find(")", idx+1)
    if idx2 < 0:
        return ''
        #raise Exception(f"Can't find name in '{s}'")
    return s[idx+1:idx2]

def main():
    normalKeyCodeMap = { }
    shiftKeyCodeMap = { }
    altGrKeyCodeMap = { }

    names = {}

    shiftCode = None
    altGrCode = None

    modmapInfo = subprocess.check_output(["xmodmap", "-pk"]).decode()
    for l in modmapInfo.splitlines():
        parts = l.split("\t")
        if len(parts) < 3:
            continue

        try:
            keyCode = int(parts[0])
        except:
            continue

        normalName = getName(parts[1])
        shiftName = getName(parts[2])
        altGrName = getName(parts[5]) if len(parts) > 5 else None

        names[normalName] = normalName
        names[shiftName] = shiftName
        names[altGrName] = altGrName

        if normalName == "Shift_L":
            shiftCode = keyCode
        elif normalName == "ISO_Level3_Shift":
            altGrCode = keyCode

        for n,m in [ (normalName, normalKeyCodeMap),
                     (shiftName, shiftKeyCodeMap),
                     (altGrName, altGrKeyCodeMap) ]:

            if n is not None:
                m[n] = keyCode

    kbdInfo = {
            "shiftcode": shiftCode,
            "altgrcode": altGrCode,
            "normalmap": normalKeyCodeMap,
            "shiftmap": shiftKeyCodeMap,
            "altgrmap": altGrKeyCodeMap,
            "charnames": charToName
    }

    print("keyMap = ")
    print(json.dumps(kbdInfo, indent=4, sort_keys=True))
    #pprint.pprint(names)




if __name__ == "__main__":
    main()
