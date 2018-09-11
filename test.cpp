#include "goldbachtester.h"

int main(int argc, char* argv[])
{

    GoldbachTester goldbachTester(argc, argv);
    goldbachTester.run();
    goldbachTester.testPercent();

    return 0;

}
