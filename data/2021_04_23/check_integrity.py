# import dask.dataframe as pd
from pandas import read_csv
from collections import Counter
import sys


def get_name_charge(string):
    string = string[0].upper() + string[1:]
    if string[-1] == "0":
        return string[:-1], None
    elif string[-1] == "1":
        if string[-2] == "-":
            return string[:-2], "-"
        else:
            return string[:-1], "+"
    else:
        raise Exception(f"String '{string}' does not contain charge information")


def check_integrity(folder_name, verbose=True):
    passed_tests = 0
    failed_tests = 0
    output = read_csv(folder_name + "/EnergyIn.csv")
    output_sums = read_csv(folder_name + "/CRY_output.csv")


    # checking particle counts
    counts = Counter(output["Particle"] + output["Charge"].astype(str))
    particle_key = counts.keys()
    particles = list(map(get_name_charge, particle_key))
    particle_counts = list(counts.values())

    for index, particle in enumerate(particles):
        particle_part = output_sums[output_sums["Particle"] == particle[0]]
        if particle[1]:
            count = list(
                particle_part[particle_part["Charge"] == particle[1]]["Count"]
            )[0]
        else:
            count = list(particle_part["Count"])[0]
        if count == particle_counts[index]:
            if verbose:
                print(f"particle count for {particle} checks out")
            passed_tests = passed_tests + 1
        else:
            failed_tests = failed_tests + 1
            if verbose:
                print(
                    f'Particle count of {particle} is off\n provided_sum: {output_sums[output_sums["Particle"] == particle]["Count"]}\n calculated_sum: {particle_counts[index]}'
                )

    # checking particle energy sum
    for key in particle_key:
        name, charge = get_name_charge( key )
        same_name = output[output["Particle"] == name.lower()]

        particle_part = output_sums[output_sums["Particle"] == name]
        if charge:
            energy = list(
                particle_part[particle_part["Charge"] == charge]["Energy"]
            )[0]
        else:
            energy = list(particle_part["Energy"])[0]


        if charge == "+":
            charge = "1"
        elif charge == "-":
            charge = "-1"
        else:
            charge = "0"
        sum_energy = sum(list(same_name[same_name["Charge"].astype(str) == charge]["energy"]))

        if sum_energy == 0 and energy == 0:
            if verbose:
                print(f"Energies summation for {name, charge} checks out")
            passed_tests = passed_tests + 1
        elif sum_energy == 0 or energy == 0:
            if verbose:
                print("error", sum_energy, energy)
            failed_tests = failed_tests + 1
        elif abs((energy - sum_energy) / sum_energy)*100 > 0.001:
            if verbose:
                print("Error", abs((energy - sum_energy) / sum_energy)*100, "%")
            failed_tests = failed_tests + 1
        else:
            if verbose:
                print(f"Energies summation for {name, charge} checks out")
            passed_tests = passed_tests + 1

    if failed_tests == 0:
        print(f"{folder_name} passed {passed_tests}=all tests")
    else:
        print(f"{folder_name} failed {failed_tests} tests out of {passed_tests + failed_tests}")


if __name__ == "__main__":
    for folder in sys.argv[1:]:
        check_integrity(folder, verbose=False)
