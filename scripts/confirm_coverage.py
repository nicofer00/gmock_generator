import xml.etree.ElementTree as ET
import sys
import os

LINE_COVERAGE_PASSING = 90
BRANCH_COVERAGE_PASSING = 75

def ParseXML(file):
    tree = ET.parse(file)
    root = tree.getroot()
    for item in root.findall('packages'):
        for package in item:
            for classes in package:
                for c in classes:
                    line_coverage = int(float(c.attrib['line-rate']) * 100)
                    branch_coverage = int(float(c.attrib['branch-rate']) * 100)
                    if (line_coverage < LINE_COVERAGE_PASSING or
                       branch_coverage < BRANCH_COVERAGE_PASSING):
                        sys.exit(1)
    sys.exit(0)
def main(filepath):
    ParseXML(filepath)
if __name__ == "__main__":
    main(sys.argv[1])
