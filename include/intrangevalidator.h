#ifndef INTRANGEVALIDATOR_H
#define INTRANGEVALIDATOR_H

#include <QValidator>

QT_BEGIN_NAMESPACE
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

        bool isOk = false;

        if (input.toInt(&isOk) > top())
        {
            return State::Invalid;
        }

        if (isOk == false)
        {
            return State::Invalid;
        }

        return State::Acceptable;
    }
};
QT_END_NAMESPACE

#endif // INTRANGEVALIDATOR_H
