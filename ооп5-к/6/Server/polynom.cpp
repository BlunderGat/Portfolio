//#include "polynom.h"
//#include <iostream>
//#include <stdexcept>
//#include <iostream>
//#include <sstream>
//#include <iomanip>

//using namespace std;

//MYpolynom::MYpolynom() : a_n(0), stepen(0), roots_array(), coefficients_array() {}

//MYpolynom::MYpolynom(number a_n, unsigned stepen, const Array& input_roots) {
////    if (a_n == 0) {
////        throw std::invalid_argument("a_n cannot be zero");
////    }
//    this->a_n = a_n;
//    if (stepen != input_roots.getSize()) {
//        throw std::invalid_argument("Array size does not match degree");
//    }
//    this->stepen = stepen;

//    roots_array.Resize(stepen);
//    coefficients_array.Resize(stepen + 1);
//    for (int i = 0; i < stepen; i++) {
//        roots_array[i] = input_roots[i];
//    }
//    Count_coeff();
//}

//void MYpolynom::addMonomial(number coeff) {
//    if (coeff == 0) return; // Skip adding zero coefficients or empty polynomial
//        if (stepen == 0 && coefficients_array.getSize() == 0)
//        {
//            coefficients_array.Resize(stepen + 1);
//            coefficients_array[stepen] = coeff;
//            roots_array.Resize(stepen + 1);
//            roots_array[stepen] = coeff;
//            stepen++;
//        }
//        else if (stepen < coefficients_array.getSize() - 1) {
//            // If we're not at the maximum degree, add the new coefficient
//            coefficients_array[stepen + 1] = coeff;
//            stepen++;
//        } else {
//            // If we're at the maximum degree, increase array size
//            coefficients_array.Resize(stepen + 1);
//            coefficients_array[stepen] = coeff;
//            roots_array.Resize(stepen + 1);
//            roots_array[stepen] = coeff;
//            stepen++;
//        }

//        // Update roots array if necessary
////        if (coeff != 0 && stepen > 0) {
////            roots_array[stepen] = 0; // Set root to 0 for constant term
////        }
//}

//void MYpolynom::removeMonomial(unsigned index) {
//    if (index >= stepen) {
//        throw std::out_of_range("Index exceeds polynomial degree");
//    }

//    if (index == stepen - 1) {
//        coefficients_array.Resize(stepen - 1);
//        roots_array.Resize(stepen - 1);
//        stepen--;
//        return;
//    }

//    for (unsigned i = index; i < stepen - 1; i++) {
//        coefficients_array[i] = coefficients_array[i + 1];
//        roots_array[i] = roots_array[i + 1];
//    }

//    coefficients_array.Resize(stepen - 1);
//    roots_array.Resize(stepen - 1);
//    stepen--;
//}



//std::string MYpolynom::toString(const MYpolynom& pol)  {
//    string str = "";

//    if (a_n.re * a_n.im != 0.0) {
//        std::stringstream ss1;
//        std::stringstream ss2;

//        double xre = pol.a_n.re;
//        double xim = pol.a_n.im;
//        ss1 << std::fixed << std::setprecision(1) << xre;
//        ss2 << std::fixed << std::setprecision(1) << xim;
//        str += ("(");
//        str +=  ss1.str() + " + " + ss2.str() + "i";
//        str+= ")";
//        if (pol.roots_array.getSize() != 0) {str += " * ";}
//        }
//    if (stepen > 0){
//        for (int i = 0; i <=  pol.stepen - 1; i++) {
//            std::stringstream ss1;
//            std::stringstream ss2;
//            complex x = pol.roots_array[i];
//            double xre = x.re;
//            double xim = x.im;
//            ss1 << std::fixed << std::setprecision(1) << xre;
//            ss2 << std::fixed << std::setprecision(1) << xim;
//            if (i != 0) {str+= " * ";}
//            str += "(x - (";
//            str += ss1.str() + " + " + ss2.str() + "i";
//            str+="))";

//        }
//    }
//        return str;
//}






//void MYpolynom::Count_coeff() {
//    coefficients_array[0] = 1;
//    for (unsigned j = 1; j <= stepen; j++) {
//        coefficients_array[j] = 0;
//    }
//    for (unsigned j = 0; j < stepen; j++) {
//        for (int k = j; k >= 0; k--) {
//            coefficients_array[k + 1] = coefficients_array[k + 1] - roots_array[j] * coefficients_array[k];
//        }
//    }
//    for (unsigned i = 0; i <= stepen; i++) {
//        coefficients_array[i] = coefficients_array[i] * a_n;
//    }
//}


//number MYpolynom::getCoefficient(unsigned pos) const {
//    if (pos > stepen) {
//        throw std::out_of_range("Index exceeds polynomial degree");
//    }
//    return coefficients_array[stepen - pos];
//}

//void MYpolynom::enterPolynom(unsigned new_stepen, number new_a_n, const Array& input_roots) {
//    if (new_a_n == 0) {
//        throw std::invalid_argument("a_n cannot be zero");
//    }
//    this->a_n = new_a_n;
//    if (new_stepen != input_roots.getSize()) {
//        throw std::invalid_argument("Array size does not match degree");
//    }
//    stepen = new_stepen;

//    roots_array.Resize(stepen);
//    coefficients_array.Resize(stepen + 1);
//    for (int i = 0; i < stepen; i++) {
//        roots_array[i] = input_roots[i];
//    }
//    Count_coeff();
//}

//void MYpolynom::changeA_n(number new_a_n) {
//    a_n.re = new_a_n.re;
//    a_n.im = new_a_n.im;
//    //Count_coeff();
//}

//number MYpolynom::getA_n()
//{
//    return a_n;
//}

//void MYpolynom::changeX(unsigned index, number new_x) {
//    if (index >= stepen) {
//        throw std::out_of_range("Invalid index");
//    }
//    roots_array[index] = new_x;
//    Count_coeff();
//}

//number MYpolynom::findValue(number x) const {
//    number val = 1;
//    for (unsigned i = 0; i < stepen; i++) {
//        val = val * (x - roots_array[i]);
//    }
//    return val * a_n;
//}

//std::ostream& operator<<(std::ostream& stream_out, const MYpolynom& pol) {
//    stream_out << "(" << pol.a_n << ")";
//    for (int i = pol.stepen - 1; i >= 0; i--) {
//        complex x = pol.roots_array[i];
//        if (x < 0) {
//            stream_out << " * (x + (" << ( x * -1 ) << "))";
//        }

//        else {
//            stream_out << " * (x - (" << x << "))";
//        }
//    }
//    return stream_out;
//}

//unsigned MYpolynom::getStepen() const {
//    return stepen;
//}

