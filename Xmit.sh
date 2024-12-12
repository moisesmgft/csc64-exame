#!/bin/bash
#
# script para simular linha de producao em OpenMP no Santos Dumont
#
# uso: Xmit.sh Omp
#    onde Omp = numero de threads OpenMP (inteiro de 1 a 24)
#
# argumentos do script
#
if [ "$#" -ne 1 ]; then
  echo "use Xmit.sh <numero de threads OpenMP>"
  exit
fi
export Omp=$1

DirBase=`pwd`
Exec=${DirBase}/build/parallel.exe
Job=parallel${Omp}
ArqSaida=${DirBase}/output/Out_${Omp}_OMP

# Compile
make all
make db

#
# gera arquivo ssub.sh
#
cat <<EOF0> ${DirBase}/ssub.sh
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=${Omp}
#SBATCH --partition=sequana_cpu_dev
#SBATCH --job-name=${Job}
#SBATCH --time=00:01:00
#SBATCH --output=${ArqSaida}
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=${Omp}
srun ${Exec}
EOF0
#
# termina arquivo ssub.sh
# envia ssub.sh para execucao
#
sbatch ssub.sh
exit