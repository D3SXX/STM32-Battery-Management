# **Git Action for Continuous Integration**

### Quick recap
Continuous integration (CI for short) is a practice used to reduce the amount of errors, shorten the time for developing a code and make the merging process easier. It involves frequent commits during the development cycle. After each commit, the server (Github Actions in our case) will build and test the code for errors. 

### Continuous integration using GitHub Actions
The GitHub Action’s workflow can work by using [Github](https://docs.github.com/en/actions/using-github-hosted-runners/about-github-hosted-runners) or [Self-hosted ](https://docs.github.com/en/actions/hosting-your-own-runners/managing-self-hosted-runners/about-self-hosted-runners) virtual machines. The workflow should be configured to run when an event occurs like committing to a repository. GitHub will validate the code by using CI tests and indicate if there is an error. When successful, the code will be able to be reviewed by other team members.

### GitHub-hosted runner usage
To use, first, the job should be created with `runs-on`. A full list of available runner images can be [found here](https://docs.github.com/en/actions/using-github-hosted-runners/about-github-hosted-runners#supported-runners-and-hardware-resources). GitHub will provision a new virtual machine for each job. 

### Workflow creation
Workflow is made by using a YAML file, in which each job script (actions and dependencies) is specified. When the job is done, the output of it can be seen in GitHub UI.

An example of a YAML file that is using Ubuntu

```
name: CI for STM32 Project

on:
  push:
    branches:
      - main

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout code
        uses: actions/checkout@v2

      - name: Set up ARM GCC toolchain
        uses: sstaub/setup-arm-embedded@v2
        with:
          version: '10'

      - name: Build STM32 project
        run: |
          cd path/to/your/stm32/project
          pio run -e nucleo_l152re -t upload
        env:
          PATH: ${{ env.ARM_EABI_PATH }}:$PATH

      - name: Run tests
        run: |
          cd path/to/your/tests
          ./run_tests.sh
```

**The most interesting syntax in this example**

- `name` sets a name for the GitHub Actions workflow
- `on` specifies when the workflow should be triggered. 
- `jobs` define the jobs that will run in the workflow.
- `build` defines a job
- `runs-on` specifies the type of virtual machine (runner) the job will run on.
- `steps` Within a job defines a series of steps that execute specific tasks.
- `name` sets a name for the step.
- `uses` specifies an action to use.
- `env` allows you to set environment variables for steps.

# References

- For terminology explanation use https://docs.github.com/en/actions/learn-github-actions/understanding-github-actions
- For CI in-depth explanation use https://docs.github.com/en/actions/automating-builds-and-tests/about-continuous-integration
- For YAML in-depth explanation use https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions
