# 4dev4d-libraries

The CMakeLists.txt is an example to get you started.

Libraries and headers for the 4DEV4D labs.
Use as-is in the synth project.

### 🛠️ How to Use 4dev4d-libraries as a Submodule
1. **Add the submodule to a project:**
   ```bash
   SUBMODULE_GIT_REPO=https://github.com/your-username/4dev4d-libraries.git
   SUBMODULE_LOCATION_IN_PROJECT=path/to/4dev4d-libraries
   git submodule add $SUBMODULE_GIT_REPO $SUBMODULE_LOCATION_IN_PROJECT
   ```
   - This will add the `4dev4d-libraries` repo into the given path in your
     project.
   - NOTE: the directory name in your project need not match
     `4dev4d-libraries`.

2. **Commit the change:**
   ```bash
   git commit -m "Added 4dev4d-libraries submodule"
   ```

3. **To clone a project with submodules:**
   ```bash
   git clone --recurse-submodules <repo-url>
   ```
   Or, after cloning:
   ```bash
   git submodule update --init --recursive
   ```

4. **To pull latest 4dev4d-libraries changes later:**
   Inside the `4dev4d-libraries` subdirectory within your project:
   ```bash
   git checkout main  # or your target branch
   git pull
   ```
   Then back in the parent repo:
   ```bash
   git add path/to/4dev4d-libraries
   git commit -m "Updated 4dev4d-libraries submodule"
   ```


