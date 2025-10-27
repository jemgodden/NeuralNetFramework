#include <iostream>
#include <mach/boolean.h>

#include "include/nnf/matrix/matrix.h"
#include "include/nnf/network/activations.h"
#include "include/nnf/utils/file_reader.h"
#include "include/nnf/utils/file_writer.h"


int main(int argc, char *argv[]) {
    const Matrix* m = new Matrix(4, 4, 2);
    m->set(7, 1, 50);
    std::cout << "INITIAL: m" << std::endl;
    m->print();

    m->scale(3);
    std::cout << "SCALE: m" << std::endl;
    m->print();

    m->addScalar(1);
    std::cout << "ADD_SCALAR: m" << std::endl;
    m->print();

    m->subtractScalar(1);
    std::cout << "SUBTRACT_SCALAR: m" << std::endl;
    m->print();

    m->setIdentity();
    std::cout << "IDENTITY: m" << std::endl;
    m->print();

    const Matrix* m2 = new Matrix(4, 4, 7);
    std::cout << "INITIAL: m2" << std::endl;
    m2->print();

    const Matrix* m3 = m->add(m2);
    std::cout << "ADD: m, m2" << std::endl;
    m3->print();

    const Matrix* m4 = m->subtract(m2);
    std::cout << "SUBTRACT: m, m2" << std::endl;
    m4->print();

    const Matrix* m5 = m->multiply(m2);
    std::cout << "MULTIPLY: m, m2" << std::endl;
    m5->print();

    const Matrix* m6 = new Matrix(2, 3, 3);
    std::cout << "INITIAL: m6" << std::endl;
    m6->print();

    const Matrix* m7 = new Matrix(3, 2, 4);
    std::cout << "INITIAL: m7" << std::endl;
    m7->print();

    const Matrix* m8 = m4->dot(m5);
    std::cout << "DOT: m4, m5" << std::endl;
    m8->print();

    const Matrix* m9 = new Matrix(4, 4);
    m9->setAscending();
    std::cout << "INITIAL: m9" << std::endl;
    m9->print();

    m9->transpose();
    std::cout << "TRANSPOSE: m9" << std::endl;
    m9->print();

    m5->apply(&square);
    std::cout << "APPLY: m5" << std::endl;
    m5->print();

    MatrixFileReader* matrix_file_reader = new MatrixFileReader();
    std::cout << "hello" << std::endl;
    const Matrix* load_matrix = matrix_file_reader->readMatrixFromFile("example_data/test.csv", TRUE);
    std::cout << "READ DATA: example_data" << std::endl;
    load_matrix->print();

    const Matrix* rand_matrix = new Matrix(4, 4);
    rand_matrix->randomise();
    std::cout << "RANDOMISED: m10" << std::endl;
    rand_matrix->print();

    MatrixFileWriter* matrix_file_writer = new MatrixFileWriter();
    matrix_file_writer->writeMatrixToFile("example_data/test2.csv", rand_matrix);

    delete m;
    delete m2;
    delete m3;
    delete m4;
    delete m5;
    delete m6;
    delete m7;
    delete m8;
    delete m9;
    delete matrix_file_reader;
    delete rand_matrix;

    return 0;
};
