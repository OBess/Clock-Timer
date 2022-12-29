#ifndef INTRANGEVALIDATOR_H
#define INTRANGEVALIDATOR_H

#include <QValidator>

QT_BEGIN_NAMESPACE
/// @brief Extends QIntValidator class to filter input number out of range
class IntRangeValidator : public QIntValidator
{
    Q_OBJECT

public:
    explicit IntRangeValidator(int min, int max, QObject *parent = nullptr)
        : QIntValidator(min, max, parent)
    {
    }

    virtual ~IntRangeValidator() override = default;

    virtual State validate(QString &input, int &pos) const override
    {
        if (input.isEmpty())
        {
            return State::Acceptable;
        }

        // If the input number is out of range, then the state is invalid
        if (bool isOk = false;
            isOk == false || input.toInt(&isOk) > top() ||
            input.toInt(&isOk) < bottom())
        {
            return State::Invalid;
        }

        return State::Acceptable;
    }
};
QT_END_NAMESPACE

#endif // INTRANGEVALIDATOR_H
