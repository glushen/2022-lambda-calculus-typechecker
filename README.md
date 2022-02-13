## Lambda calculus typechecker
[![CI](https://github.com/glushen/2022-lambda-calculus-typechecker/actions/workflows/cmake.yml/badge.svg)](https://github.com/glushen/2022-lambda-calculus-typechecker/actions/workflows/cmake.yml)
### Sample usage
```
Enter a sequent in the form [Γ⊢]t[:A] (you could use | as ⊢, \ as λ and -> as →):
x:α→β→γ, z:α, y:α→β ⊢ xz(yz) : γ
The sequent is correct.
Context: x:α→β→γ, z:α, y:α→β
Term: xz(yz)
Expected type: γ
Inferred type: γ
```
See other samples in [tests/tests.cpp](tests/tests.cpp).

### Build and run
Clone the repo and build the project:
```bash
git clone https://github.com/glushen/2022-lambda-calculus-typechecker
cd 2022-lambda-calculus-typechecker
git submodule update --init
mkdir build
cd build
cmake ..
make
```
Then run `./lambda_calculus_typechecker_run`.
