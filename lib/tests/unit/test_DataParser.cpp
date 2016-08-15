#include <CppBuildInfo/DataParser.h>
#include <gtest/gtest.h>

#include <QFile>
#include <QDir>

TEST(LibCppBuildInfoDataParser, shouldCreateInstanceWithNonExistingFileAndThrowException)
{
    const QString path = QDir::tempPath() + "/NonExistingFile.txt";
    EXPECT_THROW(DataParser instance (path), std::runtime_error);
}

TEST(LibCppBuildInfoDataParser, shouldCreateInstanceWithEmptyFileAndReadNothing)
{
    const QString path = QDir::tempPath() + "/EmptyFile.txt";
    QFile file(path);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    file.close();
    ASSERT_TRUE(QFileInfo::exists(path));

    DataParser instance (path);
    EXPECT_TRUE(instance.getFileNames().empty());
    EXPECT_TRUE(instance.getTimes().empty());
    EXPECT_TRUE(instance.getStartingTimes().empty());
    EXPECT_TRUE(instance.getEndingTimes().empty());
    EXPECT_EQ(0, instance.getNComcurrentProcesses());
}

TEST(LibCppBuildInfoDataParser, shouldCreateInstanceWithValidDataAndReadValues)
{
}

/// \todo make parseData public and returning false if the data in the file is not valid