import xml.etree.ElementTree as ET
import sys
import os

def ParseXML(file):
    tree = ET.parse(file)
    root = tree.getroot()
    if int(root.get('failures')) != 0:
        sys.exit(1)
    else:
        sys.exit(0)

def main(filepath):
    ParseXML(filepath)

if __name__ == "__main__":
    main(sys.argv[1])
