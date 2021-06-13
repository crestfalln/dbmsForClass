include(FetchContent)

FetchContent_Declare(sqliteOrm
  GIT_REPOSITORY https://github.com/fnc12/sqlite_orm
  GIT_TAG        1.6
)
add_subdirectory(sqlite_orm)