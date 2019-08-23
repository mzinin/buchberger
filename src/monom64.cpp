#include "monom64.h"

void Monom64::init()
{
    dimIndepend_ = independ->dim();
    for (int i = 0; i < 64; ++i)
    {
        one_[i] = 1;
        one_[i] = one_[i] << i;
        zero_[i] = ~one_[i];
    }

    degrees_[0] = 0; degrees_[1] = 1; degrees_[2] = 1; degrees_[3] = 2;
    degrees_[4] = 1; degrees_[5] = 2; degrees_[6] = 2; degrees_[7] = 3;
    degrees_[8] = 1; degrees_[9] = 2; degrees_[10] = 2; degrees_[11] = 3;
    degrees_[12] = 2; degrees_[13] = 3; degrees_[14] = 3; degrees_[15] = 4;
    degrees_[16] = 1; degrees_[17] = 2; degrees_[18] = 2; degrees_[19] = 3;
    degrees_[20] = 2; degrees_[21] = 3; degrees_[22] = 3; degrees_[23] = 4;
    degrees_[24] = 2; degrees_[25] = 3; degrees_[26] = 3; degrees_[27] = 4;
    degrees_[28] = 3; degrees_[29] = 4; degrees_[30] = 4; degrees_[31] = 5;
    degrees_[32] = 1; degrees_[33] = 2; degrees_[34] = 2; degrees_[35] = 3;
    degrees_[36] = 2; degrees_[37] = 3; degrees_[38] = 3; degrees_[39] = 4;
    degrees_[40] = 2; degrees_[41] = 3; degrees_[42] = 3; degrees_[43] = 4;
    degrees_[44] = 3; degrees_[45] = 4; degrees_[46] = 4; degrees_[47] = 5;
    degrees_[48] = 2; degrees_[49] = 3; degrees_[50] = 3; degrees_[51] = 4;
    degrees_[52] = 3; degrees_[53] = 4; degrees_[54] = 4; degrees_[55] = 5;
    degrees_[56] = 3; degrees_[57] = 4; degrees_[58] = 4; degrees_[59] = 5;
    degrees_[60] = 4; degrees_[61] = 5; degrees_[62] = 5; degrees_[63] = 6;
    degrees_[64] = 1; degrees_[65] = 2; degrees_[66] = 2; degrees_[67] = 3;
    degrees_[68] = 2; degrees_[69] = 3; degrees_[70] = 3; degrees_[71] = 4;
    degrees_[72] = 2; degrees_[73] = 3; degrees_[74] = 3; degrees_[75] = 4;
    degrees_[76] = 3; degrees_[77] = 4; degrees_[78] = 4; degrees_[79] = 5;
    degrees_[80] = 2; degrees_[81] = 3; degrees_[82] = 3; degrees_[83] = 4;
    degrees_[84] = 3; degrees_[85] = 4; degrees_[86] = 4; degrees_[87] = 5;
    degrees_[88] = 3; degrees_[89] = 4; degrees_[90] = 4; degrees_[91] = 5;
    degrees_[92] = 4; degrees_[93] = 5; degrees_[94] = 5; degrees_[95] = 6;
    degrees_[96] = 2; degrees_[97] = 3; degrees_[98] = 3; degrees_[99] = 4;
    degrees_[100] = 3; degrees_[101] = 4; degrees_[102] = 4; degrees_[103] = 5;
    degrees_[104] = 3; degrees_[105] = 4; degrees_[106] = 4; degrees_[107] = 5;
    degrees_[108] = 4; degrees_[109] = 5; degrees_[110] = 5; degrees_[111] = 6;
    degrees_[112] = 3; degrees_[113] = 4; degrees_[114] = 4; degrees_[115] = 5;
    degrees_[116] = 4; degrees_[117] = 5; degrees_[118] = 5; degrees_[119] = 6;
    degrees_[120] = 4; degrees_[121] = 5; degrees_[122] = 5; degrees_[123] = 6;
    degrees_[124] = 5; degrees_[125] = 6; degrees_[126] = 6; degrees_[127] = 7;
    degrees_[128] = 1; degrees_[129] = 2; degrees_[130] = 2; degrees_[131] = 3;
    degrees_[132] = 2; degrees_[133] = 3; degrees_[134] = 3; degrees_[135] = 4;
    degrees_[136] = 2; degrees_[137] = 3; degrees_[138] = 3; degrees_[139] = 4;
    degrees_[140] = 3; degrees_[141] = 4; degrees_[142] = 4; degrees_[143] = 5;
    degrees_[144] = 2; degrees_[145] = 3; degrees_[146] = 3; degrees_[147] = 4;
    degrees_[148] = 3; degrees_[149] = 4; degrees_[150] = 4; degrees_[151] = 5;
    degrees_[152] = 3; degrees_[153] = 4; degrees_[154] = 4; degrees_[155] = 5;
    degrees_[156] = 4; degrees_[157] = 5; degrees_[158] = 5; degrees_[159] = 6;
    degrees_[160] = 2; degrees_[161] = 3; degrees_[162] = 3; degrees_[163] = 4;
    degrees_[164] = 3; degrees_[165] = 4; degrees_[166] = 4; degrees_[167] = 5;
    degrees_[168] = 3; degrees_[169] = 4; degrees_[170] = 4; degrees_[171] = 5;
    degrees_[172] = 4; degrees_[173] = 5; degrees_[174] = 5; degrees_[175] = 6;
    degrees_[176] = 3; degrees_[177] = 4; degrees_[178] = 4; degrees_[179] = 5;
    degrees_[180] = 4; degrees_[181] = 5; degrees_[182] = 5; degrees_[183] = 6;
    degrees_[184] = 4; degrees_[185] = 5; degrees_[186] = 5; degrees_[187] = 6;
    degrees_[188] = 5; degrees_[189] = 6; degrees_[190] = 6; degrees_[191] = 7;
    degrees_[192] = 2; degrees_[193] = 3; degrees_[194] = 3; degrees_[195] = 4;
    degrees_[196] = 3; degrees_[197] = 4; degrees_[198] = 4; degrees_[199] = 5;
    degrees_[200] = 3; degrees_[201] = 4; degrees_[202] = 4; degrees_[203] = 5;
    degrees_[204] = 4; degrees_[205] = 5; degrees_[206] = 5; degrees_[207] = 6;
    degrees_[208] = 3; degrees_[209] = 4; degrees_[210] = 4; degrees_[211] = 5;
    degrees_[212] = 4; degrees_[213] = 5; degrees_[214] = 5; degrees_[215] = 6;
    degrees_[216] = 4; degrees_[217] = 5; degrees_[218] = 5; degrees_[219] = 6;
    degrees_[220] = 5; degrees_[221] = 6; degrees_[222] = 6; degrees_[223] = 7;
    degrees_[224] = 3; degrees_[225] = 4; degrees_[226] = 4; degrees_[227] = 5;
    degrees_[228] = 4; degrees_[229] = 5; degrees_[230] = 5; degrees_[231] = 6;
    degrees_[232] = 4; degrees_[233] = 5; degrees_[234] = 5; degrees_[235] = 6;
    degrees_[236] = 5; degrees_[237] = 6; degrees_[238] = 6; degrees_[239] = 7;
    degrees_[240] = 4; degrees_[241] = 5; degrees_[242] = 5; degrees_[243] = 6;
    degrees_[244] = 5; degrees_[245] = 6; degrees_[246] = 6; degrees_[247] = 7;
    degrees_[248] = 5; degrees_[249] = 6; degrees_[250] = 6; degrees_[251] = 7;
    degrees_[252] = 6; degrees_[253] = 7; degrees_[254] = 7; degrees_[255] = 8;
}

int Monom64::compare(const Monom64& a, const Monom64& b) const
{
    Monom64 tmp(a);
    tmp.mult(b);
    return compare(tmp);
}

std::istream& operator>>(std::istream& in, Monom64& a)
{
    std::streampos posbeg = in.tellg();
    int var = a.independ->read(in);

    if (var < 0)
    {
        in.clear();
        in.setstate(std::ios::failbit);
    }
    else
    {
        a.setZero();
        int deg;
        do
        {
            deg = 1;
            std::streampos posbeg = in.tellg();
            if ((in >> std::ws).peek() == '^')
            {
                in.get();
                in >> std::ws >> deg;
                if (in.fail() || deg < 0)
                {
                    in.setstate(std::ios::failbit);
                    IMESSAGE("expected 'degree >= 0'");
                }
            }
            a.prolong(var, deg);

            posbeg = in.tellg();
            if (in.peek() != '*')
            {
                var = -1;
            }
            else
            {
                in.get();
                var = a.independ->read(in);
                if (var < 0)
                {
                    in.clear();
                    in.seekg(posbeg);
                }
            }
        } while (var >= 0);

        if (in.eof() && deg >= 0)
        {
            in.clear();
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Monom64& a)
{
    if (a.degree() == 0)
    {
        out << '1';
    }
    else
    {
        int i = 0;
        IVariables::ConstIterator j(a.independ->begin());
        while (a.deg(i) == 0)
        {
            ++i;
            ++j;
        }

        out << *j;
        if (a.deg(i) > 1)
        {
            out << '^' << a[i];
        }

        ++i;
        ++j;

        while (j != a.independ->end())
        {
            if (a.deg(i))
            {
                out << '*' << *j;
                if (a.deg(i) > 1)
                {
                    out << '^' << a[i];
                }
            }
            ++i;
            ++j;
        }
    }
    return out;
}

IVariables* Monom64::independ = new IVariables();
IAllocator Monom64::allocator_(sizeof(Monom64));

uint64_t Monom64::zero_[64];
uint64_t Monom64::one_[64];
uint64_t Monom64::degrees_[256];

size_t Monom64::dimIndepend_;
