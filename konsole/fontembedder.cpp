#include <qfile.h>
#include <qtextstream.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

using namespace std;

static Q_UINT32 charVal(QChar val)
{
    if (val == ' ')
        return 0;
    else
        return 1;
}

static Q_UINT32 readGlyphLine(QTextStream& input)
{
    QString line = input.readLine();
    while (line.length() < 5)
        line += ' ';

    Q_UINT32 val =  charVal(line[0]) |
            (charVal(line[1]) << 1)  |
            (charVal(line[2]) << 2)  |
            (charVal(line[3]) << 3)  |
            (charVal(line[4]) << 4);
    return val;
}

static Q_UINT32 readGlyph(QTextStream& input)
{
    return readGlyphLine(input) |
            (readGlyphLine(input) << 5) |
            (readGlyphLine(input) << 10) |
            (readGlyphLine(input) << 15) |
            (readGlyphLine(input) << 20);
}

int main(int argc, char **argv)
{
    if (argc < 1)
    {
        qWarning("usage: fontembedder font.src > font.h");
        exit(1);
    }
    QFile inFile(argv[1]);
    if (!inFile.open(IO_ReadOnly))
    {
        qFatal("Can not open %s", argv[1]);
    }

    QTextStream input(&inFile);

    Q_UINT32 glyphStates[128];
    for (int i = 0; i < 128; ++i)
        glyphStates[i] = 0; //nothing..

    while (!input.atEnd())
    {
        QString line = input.readLine();
        line = line.stripWhiteSpace();
        if (line.isEmpty())
            continue; //Skip empty lines
        if (line[0] == '#')
            continue; //Skip comments

        //Must be a glyph ID.
        int glyph = line.toInt(0, 16);
        if ((glyph < 0x2500) || (glyph > 0x257f))
            qFatal("Invalid glyph number");

        glyph = glyph - 0x2500;

        glyphStates[glyph] = readGlyph(input);
    }

    //Output.
    cout<<"// WARNING: Autogenerated by \"fontembedder " << argv[1] << "\".\n";
    cout<<"// You probably do not want to hand-edit this!\n\n";
    cout<<"static const Q_UINT32 LineChars[] = {\n";

    //Nicely formatted: 8 per line, 16 lines
    for (int line = 0; line < 128; line += 8)
    {
        cout<<"\t";
        for (int col = line; col < line + 8; ++col)
        {
            cout<<"0x"<<hex<<setw(8)<<setfill('0')<<glyphStates[col];
            if (col != 127)
                cout<<", ";
        }
        cout<<"\n";
    }
    cout<<"};\n";
    return 0;
}

//kate: indent-width 4; tab-width 4; space-indent on;